#pragma once
#include "core/ITextBox.h"

class LightTextBox : public ITextBox {
public:
    void render() const override;
};
