#pragma once
#include "payment/PaymentProcessor.h"
#include "external/StripeAPI.h"

class StripeAdapter : public PaymentProcessor {
public:
    void pay(double amount) override;

private:
    StripeAPI api_;
};