#pragma once
#include <string>

class Text {
public:
    virtual std::string render() const = 0;
    virtual ~Text() = default;
};
