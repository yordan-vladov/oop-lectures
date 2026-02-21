#include "widgets/CheckBox.h"
#include "core/UIFactory.h"

#include <iostream>
#include <memory>

void CheckBox::render() const {
    std::cout << "[CheckBox] render(): drawing a selectable checkbox\n";
}

namespace {
const bool registered = UIFactory::registerType("checkbox", [] {
    return std::make_unique<CheckBox>();
});
}
