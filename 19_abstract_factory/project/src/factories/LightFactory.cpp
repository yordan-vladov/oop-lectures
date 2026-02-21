#include "factories/LightFactory.h"
#include "products/LightButton.h"
#include "products/LightCheckBox.h"
#include "products/LightTextBox.h"

std::unique_ptr<IButton> LightFactory::createButton() const {
    return std::make_unique<LightButton>();
}

std::unique_ptr<ICheckBox> LightFactory::createCheckBox() const {
    return std::make_unique<LightCheckBox>();
}

std::unique_ptr<ITextBox> LightFactory::createTextBox() const {
    return std::make_unique<LightTextBox>();
}
