#pragma once
#include "core/GUIFactory.h"

class MacFactory : public GUIFactory {
public:
    std::unique_ptr<IButton> createButton() const override;
    std::unique_ptr<ICheckBox> createCheckBox() const override;
    std::unique_ptr<ITextBox> createTextBox() const override;
};
