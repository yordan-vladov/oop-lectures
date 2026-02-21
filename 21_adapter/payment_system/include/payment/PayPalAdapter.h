#pragma once
#include "payment/PaymentProcessor.h"
#include "external/PayPalAPI.h"

class PayPalAdapter : public PaymentProcessor {
public:
    void pay(double amount) override;

private:
    PayPalAPI api_;
};