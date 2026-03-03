#pragma once
#include <memory>
#include <vector>
#include "strategies/SortStrategy.h"

class SorterContext {
public:
    SorterContext() = default;
    explicit SorterContext(std::unique_ptr<SortStrategy> s);

    void setStrategy(std::unique_ptr<SortStrategy> s);
    void sort(std::vector<int>& data);

private:
    std::unique_ptr<SortStrategy> strategy_;
};
