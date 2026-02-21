#include "UnderlineDecorator.hpp"

std::string UnderlineDecorator::render() const {
    return "<u>" + inner_->render() + "</u>";
}
