#pragma once
#include "core/UIElement.h"

class CheckBox : public UIElement {
public:
    void render() const override;
    std::string type() const override { return "checkbox"; }
};
