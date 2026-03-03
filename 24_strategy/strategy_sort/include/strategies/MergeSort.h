#pragma once
#include "SortStrategy.h"

class MergeSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) override;

private:
    static void mergesort(std::vector<int>& a, std::vector<int>& tmp, int l, int r);
    static void merge(std::vector<int>& a, std::vector<int>& tmp, int l, int m, int r);
};
