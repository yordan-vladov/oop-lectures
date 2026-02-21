#pragma once
#include "TextDecorator.hpp"

class BoldDecorator : public TextDecorator {
public:
    using TextDecorator::TextDecorator;
    std::string render() const override;
};
