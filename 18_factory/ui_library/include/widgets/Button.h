#pragma once
#include "core/UIElement.h"

class Button : public UIElement {
public:
    void render() const override;
    std::string type() const override { return "button"; }
};
