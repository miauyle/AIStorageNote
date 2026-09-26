// INTENTIONAL BUG. Run only as the opt-in AddressSanitizer exercise.
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

int main() {
    const std::uint8_t* borrowed = nullptr;
    {
        auto owner = std::make_unique<std::vector<std::uint8_t>>(4096, 42);
        borrowed = owner->data();
    } // The payload has been freed, but the borrowed address remains.
    // This represents a late callback using the address. No GPU is involved.
    std::cout << static_cast<unsigned>(borrowed[0]) << '\n';
}
