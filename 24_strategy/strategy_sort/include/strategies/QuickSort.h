#pragma once
#include "SortStrategy.h"

class QuickSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) override;

private:
    static void quicksort(std::vector<int>& a, int lo, int hi);
    static int partition(std::vector<int>& a, int lo, int hi);
};
