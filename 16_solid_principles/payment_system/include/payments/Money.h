#pragma once
#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>

namespace payments {

// Малък value type за сума в стотинки (избягваме double).
struct Money final {
    std::int64_t cents{0};

    static Money fromLeva(double leva) {
        // Проста конверсия за учебни цели (при реални системи се внимава повече).
        return Money{ static_cast<std::int64_t>(leva * 100.0 + 0.5) };
    }

    std::string toString() const {
        std::ostringstream oss;
        const auto absCents = cents < 0 ? -cents : cents;
        oss << (cents < 0 ? "-" : "")
            << (absCents / 100) << "."
            << std::setw(2) << std::setfill('0') << (absCents % 100)
            << " BGN";
        return oss.str();
    }
};

} // namespace payments
