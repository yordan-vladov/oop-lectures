#include "payments/ConsoleReceiptPrinter.h"
#include <iostream>

namespace payments {

void ConsoleReceiptPrinter::print(const std::string& text) {
    std::cout << text << "\n";
}

} // namespace payments
