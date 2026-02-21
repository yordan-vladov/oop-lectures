#pragma once
#include "TextDecorator.hpp"

class ItalicDecorator : public TextDecorator {
public:
    using TextDecorator::TextDecorator;
    std::string render() const override;
};
