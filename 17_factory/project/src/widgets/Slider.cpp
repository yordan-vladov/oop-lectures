#include "widgets/Slider.h"
#include "core/UIFactory.h"

#include <iostream>
#include <memory>

void Slider::render() const {
    std::cout << "[Slider] render(): drawing a value slider\n";
}

namespace {
const bool registered = UIFactory::registerType("slider", [] {
    return std::make_unique<Slider>();
});
}
