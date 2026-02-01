#include "factories/MacFactory.h"
#include "products/MacButton.h"
#include "products/MacCheckBox.h"
#include "products/MacTextBox.h"

std::unique_ptr<IButton> MacFactory::createButton() const {
    return std::make_unique<MacButton>();
}

std::unique_ptr<ICheckBox> MacFactory::createCheckBox() const {
    return std::make_unique<MacCheckBox>();
}

std::unique_ptr<ITextBox> MacFactory::createTextBox() const {
    return std::make_unique<MacTextBox>();
}
