#include "payments/CashPayment.h"

namespace payments {

CashPayment::CashPayment(Money cashInHand)
    : cashInHand_(cashInHand) {}

bool CashPayment::pay(Money amount) {
    if (amount.cents <= 0) return false;

    if (cashInHand_.cents < amount.cents) {
        change_ = Money{0};
        return false;
    }

    change_ = Money{ cashInHand_.cents - amount.cents };
    // За учебни цели: приемаме, че плащането "харчи" парите.
    cashInHand_ = Money{0};
    return true;
}

std::string CashPayment::name() const {
    return "Cash";
}

Money CashPayment::change() const {
    return change_;
}

} // namespace payments
