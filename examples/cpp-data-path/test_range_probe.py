"""Local HTTP contract tests. No AWS account, S3 service, or GPU is used."""
import http.server
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import threading

binary = str(Path(sys.argv[1]).resolve())
MASK = (1 << 64) - 1


def expected_byte(offset):
    x = (offset + 0x9E3779B97F4A7C15) & MASK
    x = ((x ^ (x >> 30)) * 0xBF58476D1CE4E5B9) & MASK
    x = ((x ^ (x >> 27)) * 0x94D049BB133111EB) & MASK
    return (x ^ (x >> 31)) & 255


payload = bytes(expected_byte(i) for i in range(8192))


class Handler(http.server.BaseHTTPRequestHandler):
    def log_message(self, *_):
        pass  # Avoid logging any URL, even test credentials.

    def do_GET(self):
        case = self.path.split("?", 1)[0]
        match = re.fullmatch(r"bytes=(\d+)-(\d+)", self.headers.get("Range", ""))
        if not match:
            self.send_error(400)
            return
        first, last = map(int, match.groups())
        body = payload[first:last + 1]
        status = 206
        header = f"bytes {first}-{last}/{len(payload)}"
        if case == "/corrupt":
            body = bytes([body[0] ^ 1]) + body[1:]
        elif case == "/wrong-range":
            header = f"bytes {first + 1}-{last + 1}/{len(payload)}"
        elif case == "/no-range-header":
            header = None
        elif case == "/short":
            body = body[:-1]
        elif case == "/oversize":
            body += b"x"
        elif case == "/ignored-range":
            status = 200
        elif case == "/forbidden":
            status, body = 403, b"denied"
        self.send_response(status)
        if header is not None:
            self.send_header("Content-Range", header)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)


def check(case, args, success, local=True):
    env = dict(os.environ, S3_PROBE_URL=f"http://127.0.0.1:{server.server_port}{case}?secret=do-not-log")
    command = [binary, *map(str, args)] + (["--local-test"] if local else [])
    result = subprocess.run(command, env=env, text=True, capture_output=True, timeout=5)
    if (result.returncode == 0) != success:
        raise AssertionError((case, result.returncode, result.stdout, result.stderr))
    if "do-not-log" in result.stdout + result.stderr:
        raise AssertionError("URL credential leaked")
    if success and "byte_compare=ok" not in result.stdout:
        raise AssertionError("success lacks content verification")
    print(f"PASS: {case} {args}")


server = http.server.ThreadingHTTPServer(("127.0.0.1", 0), Handler)
worker = threading.Thread(target=server.serve_forever, daemon=True)
worker.start()
try:
    check("/ok", [17, 1024, 8192], True)
    check("/ok", [8191, 1, 8192], True)
    check("/ok", [0, 8192, 8192], True)
    for case in ("/corrupt", "/wrong-range", "/no-range-header", "/short", "/oversize", "/ignored-range", "/forbidden"):
        check(case, [17, 1024, 8192], False)
    for args in ([0, 0, 8192], [8190, 3, 8192], [-1, 10, 8192], [0, 1, 67108865], ["1x", 1, 8192]):
        check("/invalid-input", args, False)
    check("/reject-plain-http", [0, 1, 8192], False, local=False)
    with tempfile.TemporaryDirectory() as directory:
        path = Path(directory) / "fixture.bin"
        subprocess.run([binary, "--generate", str(path), "8192"], check=True, capture_output=True)
        if path.read_bytes() != payload:
            raise AssertionError("generated fixture differs from independent oracle")
        again = subprocess.run([binary, "--generate", str(path), "8192"], capture_output=True)
        if again.returncode == 0:
            raise AssertionError("fixture was overwritten")
        print("PASS: fixture generation and overwrite refusal")
finally:
    server.shutdown()
    server.server_close()
    worker.join()
print("PASS: local HTTP contracts only; real S3 integration remains unverified")
