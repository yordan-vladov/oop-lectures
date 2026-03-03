#include "SorterContext.h"
#include <stdexcept>

SorterContext::SorterContext(std::unique_ptr<SortStrategy> s)
    : strategy_(std::move(s)) {}

void SorterContext::setStrategy(std::unique_ptr<SortStrategy> s) {
    strategy_ = std::move(s);
}

void SorterContext::sort(std::vector<int>& data) {
    if (!strategy_) {
        throw std::runtime_error("No sorting strategy set!");
    }
    strategy_->sort(data);
}
