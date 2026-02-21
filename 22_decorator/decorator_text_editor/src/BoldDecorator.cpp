#include "BoldDecorator.hpp"

std::string BoldDecorator::render() const {
    return "<b>" + inner_->render() + "</b>";
}
