#include "Observers/TVChannel.h"
#include <iostream>
#include <utility>

TVChannel::TVChannel(std::string name) : name_(std::move(name)) {}

void TVChannel::update(const std::string& news) {
    std::cout << "[TV - " << name_ << "] BREAKING: " << news << "\n";
}
