#pragma once
#include "Text.hpp"
#include <memory>

class TextDecorator : public Text {
public:
    explicit TextDecorator(std::unique_ptr<Text> inner);
    ~TextDecorator() override = default;

protected:
    std::unique_ptr<Text> inner_;
};
