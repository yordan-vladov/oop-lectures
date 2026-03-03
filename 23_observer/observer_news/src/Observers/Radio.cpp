#include "Observers/Radio.h"
#include <iostream>
#include <utility>

Radio::Radio(std::string station) : station_(std::move(station)) {}

void Radio::update(const std::string& news) {
    std::cout << "[Radio - " << station_ << "] News: " << news << "\n";
}
