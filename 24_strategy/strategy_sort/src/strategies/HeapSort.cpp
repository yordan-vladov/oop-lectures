#include "strategies/HeapSort.h"
#include <utility>

void HeapSort::sort(std::vector<int>& a) {
    const int n = static_cast<int>(a.size());
    if (n < 2) return;

    // Build max heap
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(a, n, i);
    }

    // Extract elements from heap
    for (int end = n - 1; end > 0; --end) {
        std::swap(a[0], a[end]);
        heapify(a, end, 0);
    }
}

void HeapSort::heapify(std::vector<int>& a, int n, int i) {
    int largest = i;
    const int l = 2 * i + 1;
    const int r = 2 * i + 2;

    if (l < n && a[l] > a[largest]) largest = l;
    if (r < n && a[r] > a[largest]) largest = r;

    if (largest != i) {
        std::swap(a[i], a[largest]);
        heapify(a, n, largest);
    }
}
