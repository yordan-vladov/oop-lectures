#pragma once

class ICheckBox {
public:
    virtual ~ICheckBox() = default;
    virtual void render() const = 0;
};
