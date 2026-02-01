#include <iostream>
#include "ApplicationState.h"

int main() {
    ApplicationState::getInstance().setUser("Ivan");

    std::cout << "Current user: "
              << ApplicationState::getInstance().getUser()
              << "\n";

    ApplicationState& a = ApplicationState::getInstance();
    ApplicationState& b = ApplicationState::getInstance();

    std::cout << "Address A: " << &a << "\n";
    std::cout << "Address B: " << &b << "\n";
    std::cout << "Same instance? "
              << std::boolalpha
              << (&a == &b)
              << "\n";

    return 0;
}
