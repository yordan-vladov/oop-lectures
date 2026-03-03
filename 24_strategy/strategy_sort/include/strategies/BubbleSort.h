#pragma once
#include "SortStrategy.h"

class BubbleSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) override;
};
