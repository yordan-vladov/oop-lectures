#pragma once
#include "Text.hpp"
#include <string>

class PlainText : public Text {
public:
    explicit PlainText(std::string text);
    std::string render() const override;

private:
    std::string text_;
};
