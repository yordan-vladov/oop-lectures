#include <iostream>
#include <vector>
#include <memory>

#include "Phone.h"
#include "Laptop.h"
#include "IRechargeable.h"

int main() {
    // Вектор от уникални указатели към Device (полиморфизъм)
    std::vector<std::unique_ptr<Device>> devices;

    devices.push_back(std::make_unique<Phone>("MyPhone", 40));
    devices.push_back(std::make_unique<Laptop>("WorkLaptop", 70));

    std::cout << "=== Стартиране на устройствата ===\n";
    for (auto& dev : devices) {
        dev->start();
    }

    std::cout << "\n=== Полиморфно зареждане (ако е възможно) ===\n";
    for (auto& dev : devices) {
        // Опит да „кастнем“ към IRechargeable
        IRechargeable* rechargeable = dynamic_cast<IRechargeable*>(dev.get());
        if (rechargeable) {
            std::cout << dev->getName()
                      << " battery before recharge: "
                      << rechargeable->batteryLevel() << "%\n";
            rechargeable->recharge();
            std::cout << dev->getName()
                      << " battery after recharge:  "
                      << rechargeable->batteryLevel() << "%\n";
        } else {
            std::cout << dev->getName()
                      << " is not rechargeable.\n";
        }
    }

    std::cout << "\n=== Спиране на устройствата ===\n";
    for (auto& dev : devices) {
        dev->stop();
    }

    // unique_ptr ще освободи паметта автоматично
    return 0;
}
