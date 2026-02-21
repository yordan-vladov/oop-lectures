#pragma once
#include "TextDecorator.hpp"

class ShadowDecorator : public TextDecorator {
public:
    using TextDecorator::TextDecorator;
    std::string render() const override;
};
