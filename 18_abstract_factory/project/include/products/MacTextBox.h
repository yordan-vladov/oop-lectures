#pragma once
#include "core/ITextBox.h"

class MacTextBox : public ITextBox {
public:
    void render() const override;
};
