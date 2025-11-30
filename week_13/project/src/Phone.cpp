#include "Phone.h"

Phone::Phone(const std::string& name, int initialBattery)
    : Device(name), battery(initialBattery)
{
}

void Phone::start() {
    std::cout << "Phone \"" << name << "\" is starting... (battery: "
              << battery << "%)\n";
}

void Phone::stop() {
    std::cout << "Phone \"" << name << "\" is shutting down.\n";
}

void Phone::recharge() {
    std::cout << "Recharging phone \"" << name << "\"...\n";
    battery = 100;
    std::cout << "Phone \"" << name << "\" is now fully charged.\n";
}

int Phone::batteryLevel() const {
    return battery;
}
