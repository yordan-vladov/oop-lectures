#include "payments/CardPayment.h"

namespace payments {

CardPayment::CardPayment(std::string maskedCard, Money dailyLimit)
    : maskedCard_(std::move(maskedCard)), dailyLimit_(dailyLimit) {}

bool CardPayment::pay(Money amount) {
    if (amount.cents <= 0) return false;

    const auto newSpent = spentToday_.cents + amount.cents;
    if (newSpent > dailyLimit_.cents) return false;

    spentToday_ = Money{ newSpent };
    return true;
}

std::string CardPayment::name() const {
    return "Card (" + maskedCard_ + ")";
}

} // namespace payments
