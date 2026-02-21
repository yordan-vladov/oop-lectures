#include "widgets/TextBox.h"
#include "core/UIFactory.h"

#include <iostream>
#include <memory>

void TextBox::render() const {
    std::cout << "[TextBox] render(): drawing an input field\n";
}

namespace {
const bool registered = UIFactory::registerType("textbox", [] {
    return std::make_unique<TextBox>();
});
}
