#pragma once
#include "core/UIElement.h"

class TextBox : public UIElement {
public:
    void render() const override;
    std::string type() const override { return "textbox"; }
};
