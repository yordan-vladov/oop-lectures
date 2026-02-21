#include "TextDecorator.hpp"
#include <utility>

TextDecorator::TextDecorator(std::unique_ptr<Text> inner)
    : inner_(std::move(inner)) {}
