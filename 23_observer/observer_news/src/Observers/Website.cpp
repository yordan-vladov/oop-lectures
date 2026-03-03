#include "Observers/Website.h"
#include <iostream>
#include <utility>

Website::Website(std::string domain) : domain_(std::move(domain)) {}

void Website::update(const std::string& news) {
    std::cout << "[Website - " << domain_ << "] New post: " << news << "\n";
}
