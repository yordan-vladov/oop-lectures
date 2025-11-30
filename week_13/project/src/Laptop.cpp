#include "Laptop.h"

Laptop::Laptop(const std::string& name, int initialBattery)
    : Device(name), battery(initialBattery)
{
}

void Laptop::start() {
    std::cout << "Laptop \"" << name << "\" is booting... (battery: "
              << battery << "%)\n";
}

void Laptop::stop() {
    std::cout << "Laptop \"" << name << "\" is shutting down.\n";
}

void Laptop::recharge() {
    std::cout << "Recharging laptop \"" << name << "\"...\n";
    battery = 100;
    std::cout << "Laptop \"" << name << "\" is now fully charged.\n";
}

int Laptop::batteryLevel() const {
    return battery;
}
