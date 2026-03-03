#include "strategies/BubbleSort.h"
#include <utility>

void BubbleSort::sort(std::vector<int>& data) {
    const std::size_t n = data.size();
    if (n < 2) return;

    for (std::size_t i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (std::size_t j = 0; j < n - 1 - i; ++j) {
            if (data[j] > data[j + 1]) {
                std::swap(data[j], data[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}
