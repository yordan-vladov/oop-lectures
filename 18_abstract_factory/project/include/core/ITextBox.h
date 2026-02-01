#pragma once

class ITextBox {
public:
    virtual ~ITextBox() = default;
    virtual void render() const = 0;
};
