#include "Phone.h"

Phone::Phone(const std::string& name, int initialBattery)
    : Device(name), battery(initialBattery)
{
}

void Phone::start() {
    std::cout << "Phone \"" << getName() << "\" is starting... (battery: "
              << battery << "%)\n";
}

void Phone::stop() {
    std::cout << "Phone \"" << getName() << "\" is shutting down.\n";
}

void Phone::recharge() {
    std::cout << "Recharging phone \"" << getName() << "\"...\n";
    battery = 100;
    std::cout << "Phone \"" << getName() << "\" is now fully charged.\n";
}

int Phone::batteryLevel() const {
    return battery;
}
