#include "payment/PayPalAdapter.h"
#include "payment/PaymentProcessorFactory.h"
#include <memory>

void PayPalAdapter::pay(double amount) {
    api_.sendPayment(amount); // адаптация: pay -> sendPayment
}

namespace {
const bool registered = PaymentProcessorFactory::registerType(
    "paypal",
    [] { return std::make_unique<PayPalAdapter>(); }
);
}