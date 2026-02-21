#include "ApplicationState.h"

ApplicationState::ApplicationState()
    : username("") {}

ApplicationState& ApplicationState::getInstance() {
    static ApplicationState instance; // Meyers Singleton
    return instance;
}

void ApplicationState::setUser(const std::string& user) {
    username = user;
}

const std::string& ApplicationState::getUser() const {
    return username;
}
