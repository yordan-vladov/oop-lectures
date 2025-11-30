#pragma once

#include <string>
#include <iostream>

class Device {
public:
    Device(const std::string& name) : name(name) {}
    virtual ~Device() = default;

    virtual void start() = 0;
    virtual void stop() = 0;

    std::string getName() const { return name; }

protected:
    std::string name;
};
