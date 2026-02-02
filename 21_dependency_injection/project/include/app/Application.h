#pragma once
#include <memory>
#include <string>

class ILogger;

class Application {
private:
    std::shared_ptr<ILogger> logger;

public:
    explicit Application(std::shared_ptr<ILogger> logger);
    void run();
    void doWork(const std::string& taskName);
};
