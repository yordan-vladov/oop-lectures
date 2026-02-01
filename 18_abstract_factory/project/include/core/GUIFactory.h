#pragma once
#include <memory>

class IButton;
class ICheckBox;
class ITextBox;

class GUIFactory {
public:
    virtual ~GUIFactory() = default;

    virtual std::unique_ptr<IButton> createButton() const = 0;
    virtual std::unique_ptr<ICheckBox> createCheckBox() const = 0;
    virtual std::unique_ptr<ITextBox> createTextBox() const = 0;
};
