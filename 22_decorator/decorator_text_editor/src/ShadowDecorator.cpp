#include "ShadowDecorator.hpp"

// Example "shadow" visualization using HTML span + inline style.
// In a real UI framework, this would be handled differently.
std::string ShadowDecorator::render() const {
    return "<span style=\"text-shadow: 1px 1px 2px #000;\">" + inner_->render() + "</span>";
}
