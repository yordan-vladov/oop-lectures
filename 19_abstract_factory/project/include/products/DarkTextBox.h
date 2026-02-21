#pragma once
#include "core/ITextBox.h"

class DarkTextBox : public ITextBox {
public:
    void render() const override;
};
