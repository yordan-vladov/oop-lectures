#include "core/GUIFactory.h"
#include "factories/WindowsFactory.h"
#include "factories/MacFactory.h"
#include "core/GUIFactory.h"
#include "core/IButton.h"
#include "core/ICheckBox.h"
#include "core/ITextBox.h"

#include "factories/WindowsFactory.h"
#include "factories/MacFactory.h"


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
    WindowsFactory windows;
    MacFactory mac;

    renderUI(windows, "Windows");
    renderUI(mac, "Mac");

    return 0;
}
