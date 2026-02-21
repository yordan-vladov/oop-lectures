#pragma once
#include "TextDecorator.hpp"

class UnderlineDecorator : public TextDecorator {
public:
    using TextDecorator::TextDecorator;
    std::string render() const override;
};
