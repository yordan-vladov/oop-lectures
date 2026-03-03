#pragma once
#include "Observer/IObserver.h"
#include <string>

class Radio : public IObserver {
public:
    explicit Radio(std::string station);
    void update(const std::string& news) override;

private:
    std::string station_;
};
