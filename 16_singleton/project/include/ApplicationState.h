#pragma once

#include <string>

class ApplicationState {
private:
    std::string username;

    ApplicationState(); // private constructor

public:
    ApplicationState(const ApplicationState&) = delete;
    ApplicationState& operator=(const ApplicationState&) = delete;

    ApplicationState(ApplicationState&&) = delete;
    ApplicationState& operator=(ApplicationState&&) = delete;

    static ApplicationState& getInstance();

    void setUser(const std::string& user);
    const std::string& getUser() const;
};
