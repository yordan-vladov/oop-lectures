#pragma once
#include "IPaymentMethod.h"
#include <string>

namespace payments {

class CardPayment final : public IPaymentMethod {
public:
    CardPayment(std::string maskedCard, Money dailyLimit);

    bool pay(Money amount) override;
    std::string name() const override;

private:
    std::string maskedCard_;
    Money dailyLimit_;
    Money spentToday_{0};
};

} // namespace payments
