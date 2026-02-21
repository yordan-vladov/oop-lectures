#include "PlainText.hpp"
#include <utility>

PlainText::PlainText(std::string text)
    : text_(std::move(text)) {}

std::string PlainText::render() const {
    return text_;
}
