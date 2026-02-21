#include "payment/CryptoAdapter.h"
#include "payment/PaymentProcessorFactory.h"
#include <memory>
#include <utility>

CryptoAdapter::CryptoAdapter(std::string wallet)
    : wallet_(std::move(wallet)) {}

void CryptoAdapter::pay(double amount) {
    api_.transfer(wallet_, amount); // адаптация: pay -> transfer(wallet, amount)
}

namespace {
const bool registered = PaymentProcessorFactory::registerType(
    "crypto",
    [] { return std::make_unique<CryptoAdapter>("bg-wallet-001"); }
);
}