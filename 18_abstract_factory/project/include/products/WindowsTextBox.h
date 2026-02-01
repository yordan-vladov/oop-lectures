#pragma once
#include "core/ITextBox.h"

class WindowsTextBox : public ITextBox {
public:
    void render() const override;
};
