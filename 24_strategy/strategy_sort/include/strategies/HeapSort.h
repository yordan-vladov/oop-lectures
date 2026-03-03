#pragma once
#include "SortStrategy.h"

class HeapSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) override;

private:
    static void heapify(std::vector<int>& a, int n, int i);
};
