#pragma once

class IRechargeable {
public:
    virtual ~IRechargeable() = default;

    virtual void recharge() = 0;
    virtual int batteryLevel() const = 0;
};
