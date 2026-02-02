#include "payments/PaymentProcessor.h"
#include <sstream>

namespace payments {

PaymentProcessor::PaymentProcessor(IPaymentMethod& method, IReceiptPrinter& printer)
    : method_(method), printer_(printer) {}

bool PaymentProcessor::checkout(const std::string& orderId, Money total) {
    std::ostringstream receipt;
    receipt << "Order: " << orderId << "\n"
            << "Total: " << total.toString() << "\n"
            << "Method: " << method_.name() << "\n";

    const bool ok = method_.pay(total);
    receipt << "Status: " << (ok ? "PAID" : "DECLINED") << "\n";

    printer_.print(receipt.str());
    return ok;
}

} // namespace payments
