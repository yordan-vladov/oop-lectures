#pragma once
#include <string>
#include "Money.h"

namespace payments {

// ISP: малък интерфейс само за плащане (без несвързани методи).
class IPaymentMethod {
public:
    virtual ~IPaymentMethod() = default;

    // Връща true при успешно плащане.
    virtual bool pay(Money amount) = 0;

    // Име на метода за разписка/лог.
    virtual std::string name() const = 0;
};

} // namespace payments
