#pragma once
#include <string>

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(const std::string& news) = 0;
};
