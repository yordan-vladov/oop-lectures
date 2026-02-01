#pragma once

#include "Device.h"
#include "IRechargeable.h"

class Phone : public Device, public IRechargeable {
public:
    Phone(const std::string& name, int initialBattery = 50);

    void start() override;
    void stop() override;

    void recharge() override;
    int batteryLevel() const override;

private:
    int battery; // 0–100
};
