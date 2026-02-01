#pragma once

#include "Device.h"
#include "IRechargeable.h"

class Laptop : public Device, public IRechargeable {
public:
    Laptop(const std::string& name, int initialBattery = 80);

    void start() override;
    void stop() override;

    void recharge() override;
    int batteryLevel() const override;

private:
    int battery; // 0–100
};
