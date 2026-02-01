#include "core/UIFactory.h"
#include "core/UIElement.h"

#include <iostream>
#include <vector>

int main() {
    std::cout << "Registered UI element types:\n";
    for (const auto& t : UIFactory::registeredTypes()) {
        std::cout << " - " << t << "\n";
    }

    std::cout << "\nCreating and rendering elements:\n";

    std::vector<std::string> toCreate = {
        "button", "textbox", "checkbox", "slider", "unknown"
    };

    for (const auto& type : toCreate) {
        auto element = UIFactory::createElement(type);
        if (!element) {
            std::cout << "[Factory] Unknown type: " << type << "\n";
            continue;
        }

        std::cout << "Created: " << element->type() << "\n";
        element->render();
    }

    return 0;
}
