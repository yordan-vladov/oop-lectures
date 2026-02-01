#include "widgets/Button.h"
#include "core/UIFactory.h"

#include <iostream>
#include <memory>

void Button::render() const {
    std::cout << "[Button] render(): drawing a clickable button\n";
}

namespace {
const bool registered = UIFactory::registerType("button", [] {
    return std::make_unique<Button>();
});
}
