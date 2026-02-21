#include "payment/PaymentProcessorFactory.h"
#include "payment/PaymentProcessor.h"

bool PaymentProcessorFactory::registerType(const std::string& type, Creator creator) {
    registry()[type] = std::move(creator);
    return true;
}

std::unique_ptr<PaymentProcessor> PaymentProcessorFactory::create(const std::string& type) {
    auto it = registry().find(type);
    if (it != registry().end()) {
        return it->second();
    }
    return nullptr;
}

std::unordered_map<std::string, PaymentProcessorFactory::Creator>&
PaymentProcessorFactory::registry() {
    static std::unordered_map<std::string, Creator> instance;
    return instance;
}