#pragma once
#include <string>
#include "Money.h"
#include "IPaymentMethod.h"
#include "IReceiptPrinter.h"

namespace payments {

// SRP: този клас се грижи само за "процесиране на плащане" и изготвяне на текст.
// DIP: зависи от абстракции (IPaymentMethod, IReceiptPrinter), не от конкретни класове.
class PaymentProcessor final {
public:
    PaymentProcessor(IPaymentMethod& method, IReceiptPrinter& printer);

    bool checkout(const std::string& orderId, Money total);

private:
    IPaymentMethod& method_;
    IReceiptPrinter& printer_;
};

} // namespace payments
