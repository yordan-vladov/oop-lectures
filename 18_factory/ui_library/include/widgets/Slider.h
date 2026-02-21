#pragma once
#include "core/UIElement.h"

class Slider : public UIElement {
public:
    void render() const override;
    std::string type() const override { return "slider"; }
};
