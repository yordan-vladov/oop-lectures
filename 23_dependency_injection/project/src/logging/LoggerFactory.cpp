#include "logging/LoggerFactory.h"
#include "logging/ILogger.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

// Реализация 1: ConsoleLogger (скрит тип в cpp)
class ConsoleLogger final : public ILogger {
public:
    void log(const std::string& message) override {
        std::cout << "[console] " << message << "\n";
    }
};

// Реализация 2: FileLogger (скрит тип в cpp)
class FileLogger final : public ILogger {
private:
    std::ofstream out;

public:
    explicit FileLogger(const std::string& filePath)
        : out(filePath, std::ios::app)
    {
        if (!out) {
            throw std::runtime_error("Cannot open log file: " + filePath);
        }
    }

    void log(const std::string& message) override {
        out << "[file] " << message << "\n";
        out.flush();
    }
};

std::shared_ptr<ILogger> makeConsoleLogger() {
    return std::make_shared<ConsoleLogger>();
}

std::shared_ptr<ILogger> makeFileLogger(const std::string& path) {
    return std::make_shared<FileLogger>(path);
}
