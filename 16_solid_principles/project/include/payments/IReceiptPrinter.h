#pragma once
#include <string>

namespace payments {

// ISP: отделен интерфейс за печат на разписка.
class IReceiptPrinter {
public:
    virtual ~IReceiptPrinter() = default;
    virtual void print(const std::string& text) = 0;
};

} // namespace payments
