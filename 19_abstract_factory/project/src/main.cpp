#include "core/GUIFactory.h"
#include "factories/DarkFactory.h"
#include "factories/LightFactory.h"
#include "core/GUIFactory.h"
#include "core/IButton.h"
#include "core/ICheckBox.h"
#include "core/ITextBox.h"

#include "factories/DarkFactory.h"
#include "factories/LightFactory.h"


#include <iostream>
#include <memory>
#include <string>

static void renderUI(const GUIFactory& factory, const std::string& name) {
    std::cout << "---- Rendering UI using " << name << " factory ----\n";

    auto btn = factory.createButton();
    auto cb  = factory.createCheckBox();
    auto tb  = factory.createTextBox();

    btn->render();
    cb->render();
    tb->render();

    std::cout << "\n";
}

int main() {
    DarkFactory dark;
    LightFactory light;

    renderUI(dark, "Dark");
    renderUI(light, "Light");

    return 0;
}
