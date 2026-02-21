#include "payment/PaymentProcessorFactory.h"
#include "payment/PaymentProcessor.h"
#include <iostream>
#include <memory>
#include <string>

static void demoPay(const std::string& processorType, double amount) {
    auto processor = PaymentProcessorFactory::create(processorType);
    if (!processor) {
        std::cout << "Unknown processor type: " << processorType << "\n";
        return;
    }
    processor->pay(amount);
}

int main() {
    demoPay("paypal", 25.50);
    demoPay("stripe", 99.99);

    // Допълнително предизвикателство:
    // Crypto се добавя само чрез нов Adapter + registration, без промяна на main().
    demoPay("crypto", 0.0123);

    return 0;
}