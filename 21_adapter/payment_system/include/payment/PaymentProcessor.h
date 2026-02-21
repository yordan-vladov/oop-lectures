#pragma once
#include <string>

class PaymentProcessor {
public:
    virtual ~PaymentProcessor() = default;
    virtual void pay(double amount) = 0;
};