#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include "payment/PaymentProcessor.h"

class PaymentProcessor;

class PaymentProcessorFactory {
public:
    using Creator = std::function<std::unique_ptr<PaymentProcessor>()>;

    static bool registerType(const std::string& type, Creator creator);
    static std::unique_ptr<PaymentProcessor> create(const std::string& type);

private:
    static std::unordered_map<std::string, Creator>& registry();
};