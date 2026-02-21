#include "payments/Money.h"
#include "payments/CashPayment.h"
#include "payments/CardPayment.h"
#include "payments/ConsoleReceiptPrinter.h"
#include "payments/PaymentProcessor.h"

using payments::Money;

int main() {
    payments::ConsoleReceiptPrinter printer;

    // Пример 1: Плащане в брой
    payments::CashPayment cash(Money::fromLeva(50.00));
    payments::PaymentProcessor cashProcessor(cash, printer);
    cashProcessor.checkout("ORD-1001", Money::fromLeva(29.90));

    // Пример 2: Плащане с карта (лимит за деня)
    payments::CardPayment card("**** 1234", Money::fromLeva(100.00));
    payments::PaymentProcessor cardProcessor(card, printer);
    cardProcessor.checkout("ORD-1002", Money::fromLeva(80.00));
    cardProcessor.checkout("ORD-1003", Money::fromLeva(30.00)); // вероятно ще бъде отказано (лимит)

    return 0;
}
