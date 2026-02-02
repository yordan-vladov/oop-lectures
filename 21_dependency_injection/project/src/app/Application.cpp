#include "app/Application.h"
#include "logging/ILogger.h"
#include <stdexcept>

Application::Application(std::shared_ptr<ILogger> logger)
    : logger(std::move(logger))
{
    if (!this->logger) {
        throw std::invalid_argument("Application requires a valid ILogger instance.");
    }
}

void Application::run() {
    logger->log("Application started.");
    doWork("Load configuration");
    doWork("Process data");
    doWork("Save results");
    logger->log("Application finished.");
}

void Application::doWork(const std::string& taskName) {
    logger->log("Working on task: " + taskName);
}
