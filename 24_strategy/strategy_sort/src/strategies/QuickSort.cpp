#include "strategies/QuickSort.h"
#include <utility>

void QuickSort::sort(std::vector<int>& data) {
    if (data.size() < 2) return;
    quicksort(data, 0, static_cast<int>(data.size()) - 1);
}

void QuickSort::quicksort(std::vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;
    const int p = partition(a, lo, hi);
    quicksort(a, lo, p - 1);
    quicksort(a, p + 1, hi);
}

int QuickSort::partition(std::vector<int>& a, int lo, int hi) {
    // Lomuto partition (pivot = last)
    const int pivot = a[hi];
    int i = lo;

    for (int j = lo; j < hi; ++j) {
        if (a[j] < pivot) {
            std::swap(a[i], a[j]);
            ++i;
        }
    }
    std::swap(a[i], a[hi]);
    return i;
}
