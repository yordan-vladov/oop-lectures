#include "ItalicDecorator.hpp"

std::string ItalicDecorator::render() const {
    return "<i>" + inner_->render() + "</i>";
}
