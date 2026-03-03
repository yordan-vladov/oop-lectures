#pragma once
#include "Observer/IObserver.h"
#include <string>

class MobileApp : public IObserver {
public:
    explicit MobileApp(std::string user);
    void update(const std::string& news) override;

private:
    std::string user_;
};
