#include "Observers/MobileApp.h"
#include <iostream>
#include <utility>

MobileApp::MobileApp(std::string user) : user_(std::move(user)) {}

void MobileApp::update(const std::string& news) {
    std::cout << "[MobileApp - " << user_ << "] Push notification: " << news << "\n";
}
