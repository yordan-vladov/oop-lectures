#include "factories/DarkFactory.h"
#include "products/DarkButton.h"
#include "products/DarkCheckBox.h"
#include "products/DarkTextBox.h"

std::unique_ptr<IButton> DarkFactory::createButton() const {
    return std::make_unique<DarkButton>();
}

std::unique_ptr<ICheckBox> DarkFactory::createCheckBox() const {
    return std::make_unique<DarkCheckBox>();
}

std::unique_ptr<ITextBox> DarkFactory::createTextBox() const {
    return std::make_unique<DarkTextBox>();
}
