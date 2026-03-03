#pragma once
#include "Observer/IObserver.h"
#include <string>

class TVChannel : public IObserver {
public:
    explicit TVChannel(std::string name);
    void update(const std::string& news) override;

private:
    std::string name_;
};
