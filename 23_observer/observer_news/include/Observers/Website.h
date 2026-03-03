#pragma once
#include "Observer/IObserver.h"
#include <string>

class Website : public IObserver {
public:
    explicit Website(std::string domain);
    void update(const std::string& news) override;

private:
    std::string domain_;
};
