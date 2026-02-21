#pragma once
#include "payment/PaymentProcessor.h"
#include "external/CryptoAPI.h"
#include <string>

class CryptoAdapter : public PaymentProcessor {
public:
    explicit CryptoAdapter(std::string wallet = "default-wallet");
    void pay(double amount) override;

private:
    CryptoAPI api_;
    std::string wallet_;
};