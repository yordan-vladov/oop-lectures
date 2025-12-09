#include "Laptop.h"

Laptop::Laptop(const std::string& name, int initialBattery)
    : Device(name), battery(initialBattery)
{
}

void Laptop::start() {
    std::cout << "Laptop \"" << getName() << "\" is booting... (battery: "
              << battery << "%)\n";
}

void Laptop::stop() {
    std::cout << "Laptop \"" << getName() << "\" is shutting down.\n";
}

void Laptop::recharge() {
    std::cout << "Recharging laptop \"" << getName() << "\"...\n";
    battery = 100;
    std::cout << "Laptop \"" << getName() << "\" is now fully charged.\n";
}

int Laptop::batteryLevel() const {
    return battery;
}
