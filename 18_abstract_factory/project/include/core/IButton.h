#pragma once

class IButton {
public:
    virtual ~IButton() = default;
    virtual void render() const = 0;
};
