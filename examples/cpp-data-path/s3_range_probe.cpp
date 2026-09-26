// Small read-only probe, not a full S3 SDK or an RDMA implementation.
#include <curl/curl.h>
#include <algorithm>
#include <chrono>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using Clock = std::chrono::steady_clock;
constexpr std::uint64_t max_bytes = 64ULL * 1024 * 1024;

std::uint64_t number(const std::string& value) {
    if (value.empty() || !std::all_of(value.begin(), value.end(),
        [](unsigned char c) { return std::isdigit(c); }))
        throw std::runtime_error("expected an unsigned decimal integer");
    std::size_t used = 0;
    auto n = std::stoull(value, &used);
    if (used != value.size()) throw std::runtime_error("invalid integer");
    return n;
}

// A position-dependent, non-cryptographic fixture. The full expected slice is
// compared byte for byte; this does NOT implement an S3 checksum or an ETag.
std::uint8_t expected_byte(std::uint64_t offset) {
    std::uint64_t x = offset + 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return static_cast<std::uint8_t>(x ^ (x >> 31));
}

struct Response {
    std::vector<std::uint8_t> bytes;
    std::size_t limit;
    std::string content_range;
};

std::size_t body_callback(char* data, std::size_t size, std::size_t count, void* user) noexcept {
    auto& r = *static_cast<Response*>(user);
    if (size != 0 && count > std::numeric_limits<std::size_t>::max() / size) return 0;
    const auto n = size * count;
    if (r.bytes.size() > r.limit || n > r.limit - r.bytes.size()) return 0;
    try {
        r.bytes.insert(r.bytes.end(), data, data + n);
        return n;
    } catch (...) { return 0; } // Never unwind a C callback.
}

std::size_t header_callback(char* data, std::size_t size, std::size_t count, void* user) noexcept {
    if (size != 0 && count > std::numeric_limits<std::size_t>::max() / size) return 0;
    const auto n = size * count;
    try {
        auto& r = *static_cast<Response*>(user);
        std::string line(data, n);
        if (line.rfind("HTTP/", 0) == 0) r.content_range.clear();
        const auto colon = line.find(':');
        if (colon == std::string::npos) return n;
        auto name = line.substr(0, colon);
        std::transform(name.begin(), name.end(), name.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (name == "content-range") {
            auto value = line.substr(colon + 1);
            auto first = value.find_first_not_of(" \t\r\n");
            auto last = value.find_last_not_of(" \t\r\n");
            r.content_range = first == std::string::npos ? "" : value.substr(first, last - first + 1);
        }
        return n;
    } catch (...) { return 0; }
}

struct CurlGlobal {
    CurlGlobal() {
        if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK)
            throw std::runtime_error("curl global init failed");
    }
    ~CurlGlobal() { curl_global_cleanup(); }
};

template<class T> void set(CURL* handle, CURLoption option, T value) {
    if (curl_easy_setopt(handle, option, value) != CURLE_OK)
        throw std::runtime_error("curl option setup failed");
}

int main(int argc, char** argv) {
    try {
        if (argc == 4 && std::string(argv[1]) == "--generate") {
            const auto n = number(argv[3]);
            if (n == 0 || n > max_bytes) throw std::runtime_error("fixture size must be 1..64 MiB");
            if (std::ifstream(argv[2]).good()) throw std::runtime_error("refusing to overwrite fixture");
            std::ofstream out(argv[2], std::ios::binary);
            if (!out) throw std::runtime_error("cannot create fixture");
            for (std::uint64_t i = 0; i < n; ++i) out.put(static_cast<char>(expected_byte(i)));
            out.close();
            if (!out) throw std::runtime_error("fixture write failed");
            std::cout << "fixture_bytes=" << n << '\n';
            return 0;
        }
        if (argc < 4 || argc > 5)
            throw std::runtime_error("usage: s3_range_probe START LENGTH OBJECT_SIZE [--local-test]; URL via S3_PROBE_URL");
        const auto start = number(argv[1]);
        const auto length = number(argv[2]);
        const auto total = number(argv[3]);
        if (total == 0 || total > max_bytes || length == 0 || start >= total || length > total - start)
            throw std::runtime_error("range must be nonempty and inside a 1..64 MiB fixture");
        const bool local = argc == 5 && std::string(argv[4]) == "--local-test";
        if (argc == 5 && !local) throw std::runtime_error("unknown option");
        const char* raw = std::getenv("S3_PROBE_URL");
        if (!raw || !*raw) throw std::runtime_error("S3_PROBE_URL is required; never paste it in reports");
        std::string url(raw);
        if (local) {
            if (url.rfind("http://127.0.0.1:", 0) != 0 || url.find('@') != std::string::npos)
                throw std::runtime_error("local test accepts only http://127.0.0.1:PORT");
        } else if (url.rfind("https://", 0) != 0) {
            throw std::runtime_error("real endpoint requires HTTPS");
        }
        CurlGlobal global;
        using Handle = std::unique_ptr<CURL, decltype(&curl_easy_cleanup)>;
        Handle handle(curl_easy_init(), &curl_easy_cleanup);
        if (!handle) throw std::runtime_error("curl easy init failed");
        Response response{{}, static_cast<std::size_t>(length), ""};
        response.bytes.reserve(response.limit);
        const auto end = start + length - 1;
        const std::string range = std::to_string(start) + "-" + std::to_string(end);
        set(handle.get(), CURLOPT_URL, url.c_str());
        set(handle.get(), CURLOPT_RANGE, range.c_str());
        set(handle.get(), CURLOPT_PROTOCOLS_STR, local ? "http" : "https");
        set(handle.get(), CURLOPT_FOLLOWLOCATION, 0L); // No signed-URL redirection.
        set(handle.get(), CURLOPT_CONNECTTIMEOUT_MS, 5000L);
        set(handle.get(), CURLOPT_TIMEOUT_MS, 30000L);
        set(handle.get(), CURLOPT_NOSIGNAL, 1L);
        set(handle.get(), CURLOPT_SSL_VERIFYPEER, 1L);
        set(handle.get(), CURLOPT_SSL_VERIFYHOST, 2L);
        if (local) set(handle.get(), CURLOPT_NOPROXY, "*");
        set(handle.get(), CURLOPT_WRITEFUNCTION, &body_callback);
        set(handle.get(), CURLOPT_WRITEDATA, &response);
        set(handle.get(), CURLOPT_HEADERFUNCTION, &header_callback);
        set(handle.get(), CURLOPT_HEADERDATA, &response);

        const auto begin = Clock::now();
        const auto code = curl_easy_perform(handle.get());
        const auto received = Clock::now();
        long status = 0;
        if (curl_easy_getinfo(handle.get(), CURLINFO_RESPONSE_CODE, &status) != CURLE_OK)
            throw std::runtime_error("cannot read HTTP status");
        // Report no response body, URL, auth headers, or libcurl error-buffer text.
        if (code != CURLE_OK) throw std::runtime_error("transport/body failure; curl_code=" + std::to_string(code));
        if (status != 206) throw std::runtime_error("Range GET requires HTTP 206; got " + std::to_string(status));
        const std::string expected_range = "bytes " + range + "/" + std::to_string(total);
        if (response.content_range != expected_range) throw std::runtime_error("Content-Range mismatch");
        if (response.bytes.size() != length) throw std::runtime_error("body length mismatch");
        for (std::size_t i = 0; i < response.bytes.size(); ++i)
            if (response.bytes[i] != expected_byte(start + i)) throw std::runtime_error("payload mismatch");
        const auto verified = Clock::now();
        auto ms = [](auto a, auto b) { return std::chrono::duration<double, std::milli>(b - a).count(); };
        std::cout << std::fixed << std::setprecision(3)
            << "mode=" << (local ? "local-http-fixture" : "https-endpoint")
            << " bytes=" << length << " http=206 content_range=ok byte_compare=ok"
            << " request_ms=" << ms(begin, received)
            << " verify_ms=" << ms(received, verified)
            << " host_verified_ms=" << ms(begin, verified) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "FAIL: " << e.what() << '\n';
        return 1;
    }
}
