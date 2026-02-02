#pragma once
#include "IPaymentMethod.h"

namespace payments {

class CashPayment final : public IPaymentMethod {
public:
    explicit CashPayment(Money cashInHand);

    bool pay(Money amount) override;
    std::string name() const override;

    Money change() const;

private:
    Money cashInHand_;
    Money change_{0};
};

} // namespace payments
