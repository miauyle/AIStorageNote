"""Validate local rendered links; no network or external-source claims."""
from html.parser import HTMLParser
from pathlib import Path
import sys
from urllib.parse import unquote, urljoin, urlsplit


class Page(HTMLParser):
    def __init__(self, text):
        super().__init__(convert_charrefs=True)
        self.ids = set()
        self.links = []
        self.feed(text)

    def handle_starttag(self, tag, attrs):
        attrs = dict(attrs)
        if "id" in attrs:
            self.ids.add(attrs["id"])
        if tag == "a" and attrs.get("name"):
            self.ids.add(attrs["name"])
        if tag == "a" and attrs.get("href"):
            self.links.append(attrs["href"])


def main():
    root = Path(sys.argv[1]).resolve()
    base = sys.argv[2].rstrip("/")
    pages = {p: Page(p.read_text()) for p in root.rglob("*.html")}
    selected = [p for p in pages if p.relative_to(root).parts[0] == "docs"
                or p.relative_to(root).as_posix() in ("index.html", "updates/index.html", "SOURCES.html")]
    if not selected:
        raise SystemExit("No rendered documentation pages found")
    checked, errors = 0, []
    for source in selected:
        current = base + "/" + source.relative_to(root).as_posix()
        for href in pages[source].links:
            if href == "#" or urlsplit(href).scheme or href.startswith("//"):
                continue
            url = urlsplit(urljoin(current, href))
            path = unquote(url.path)
            if base and not (path == base or path.startswith(base + "/")):
                errors.append(f"{source.relative_to(root)}: outside baseurl: {href}")
                continue
            target = root / path[len(base):].lstrip("/")
            if target.is_dir():
                target /= "index.html"
            target = target.resolve()
            if not target.is_relative_to(root) or not target.is_file():
                errors.append(f"{source.relative_to(root)}: missing file: {href}")
            elif url.fragment and target.suffix == ".html" and unquote(url.fragment) not in pages.get(target, Page("")).ids:
                errors.append(f"{source.relative_to(root)}: missing anchor: {href}")
            checked += 1
    if errors:
        raise SystemExit("\n".join(errors))
    print(f"PASS: {checked} internal links/anchors across {len(selected)} documentation pages")


if __name__ == "__main__":
    main()
