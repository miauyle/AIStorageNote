#include <cstdint>
#include <future>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

using Bytes = std::unique_ptr<std::vector<std::uint8_t>>;

void require(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

// CPU-only exercise. Future completion here says nothing about CUDA or RDMA.
std::future<Bytes> start_copy(std::shared_future<void> permission_to_read) {
    auto source = std::make_unique<std::vector<std::uint8_t>>(4096, 42);
    return std::async(std::launch::async,
        [source = std::move(source), permission_to_read]() mutable {
            permission_to_read.get();
            auto destination = std::make_unique<std::vector<std::uint8_t>>(*source);
            return destination; // Destination ownership goes to future/consumer.
        });
} // The source is owned by the task even after this function returns.

int main() {
    try {
        auto owner = std::make_unique<std::vector<std::uint8_t>>(1024, 7);
        const auto* original_address = owner->data();
        auto next_owner = std::move(owner);
        require(!owner, "moved unique_ptr must be empty");
        require(next_owner->data() == original_address, "payload address changed");
        require(next_owner->at(0) == 7, "payload changed");
        std::cout << "ownership: moved owner, same payload address\n";

        std::promise<void> allow_read;
        auto result = start_copy(allow_read.get_future().share());
        std::cout << "async: submit function returned before copy was allowed\n";
        allow_read.set_value();
        auto consumer = result.get(); // Propagates worker exceptions too.
        require(consumer->size() == 4096, "incorrect destination size");
        for (auto value : *consumer) require(value == 42, "corrupt byte");
        std::cout << "consumer: 4096 bytes verified; releasing destination\n";
        consumer.reset();
        std::cout << "PASS: CPU ownership and async lifetime\n";
    } catch (const std::exception& e) {
        std::cerr << "FAIL: " << e.what() << '\n';
        return 1;
    }
}
