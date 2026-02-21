#pragma once
#include <string>

class UIElement {
public:
    virtual ~UIElement() = default;

    virtual void render() const = 0;
    virtual std::string type() const = 0;
};
