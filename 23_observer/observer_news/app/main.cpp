#include "Observer/NewsAgency.h"
#include "Observers/TVChannel.h"
#include "Observers/Radio.h"
#include "Observers/MobileApp.h"
#include "Observers/Website.h"

#include <memory>

int main() {
    NewsAgency agency;

    auto tv = std::make_shared<TVChannel>("BNT");
    auto radio = std::make_shared<Radio>("Darik");
    auto app = std::make_shared<MobileApp>("Ivan");
    auto site = std::make_shared<Website>("news.bg"); // допълнително предизвикателство

    agency.attach(tv);
    agency.attach(radio);
    agency.attach(app);
    agency.attach(site);

    agency.publishNews("Новината на деня: Observer pattern в C++!");
    agency.publishNews("Втора новина: smart pointers + weak_ptr за безопасност.");

    // Пример: отписване
    agency.detach(radio);
    agency.publishNews("Следваща новина: Radio вече не получава обновления.");

    return 0;
}
