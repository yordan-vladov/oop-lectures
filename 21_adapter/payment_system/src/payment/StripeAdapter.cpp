#include "payment/StripeAdapter.h"
#include "payment/PaymentProcessorFactory.h"
#include <memory>

void StripeAdapter::pay(double amount) {
    api_.makeCharge(amount); // адаптация: pay -> makeCharge
}

namespace {
const bool registered = PaymentProcessorFactory::registerType(
    "stripe",
    [] { return std::make_unique<StripeAdapter>(); }
);
}