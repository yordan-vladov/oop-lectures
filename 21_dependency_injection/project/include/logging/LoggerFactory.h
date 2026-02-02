#pragma once
#include <memory>
#include <string>

class ILogger;

std::shared_ptr<ILogger> makeConsoleLogger();
std::shared_ptr<ILogger> makeFileLogger(const std::string& path);
