#pragma once
#include "IReceiptPrinter.h"

namespace payments {

class ConsoleReceiptPrinter final : public IReceiptPrinter {
public:
    void print(const std::string& text) override;
};

} // namespace payments
