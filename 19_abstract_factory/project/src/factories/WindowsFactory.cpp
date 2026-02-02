#include "factories/WindowsFactory.h"
#include "products/WindowsButton.h"
#include "products/WindowsCheckBox.h"
#include "products/WindowsTextBox.h"

std::unique_ptr<IButton> WindowsFactory::createButton() const {
    return std::make_unique<WindowsButton>();
}

std::unique_ptr<ICheckBox> WindowsFactory::createCheckBox() const {
    return std::make_unique<WindowsCheckBox>();
}

std::unique_ptr<ITextBox> WindowsFactory::createTextBox() const {
    return std::make_unique<WindowsTextBox>();
}
