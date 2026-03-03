#include "strategies/MergeSort.h"
#include <vector>

void MergeSort::sort(std::vector<int>& data) {
    if (data.size() < 2) return;
    std::vector<int> tmp(data.size());
    mergesort(data, tmp, 0, static_cast<int>(data.size()) - 1);
}

void MergeSort::mergesort(std::vector<int>& a, std::vector<int>& tmp, int l, int r) {
    if (l >= r) return;
    const int m = l + (r - l) / 2;
    mergesort(a, tmp, l, m);
    mergesort(a, tmp, m + 1, r);
    merge(a, tmp, l, m, r);
}

void MergeSort::merge(std::vector<int>& a, std::vector<int>& tmp, int l, int m, int r) {
    int i = l;
    int j = m + 1;
    int k = l;

    while (i <= m && j <= r) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else tmp[k++] = a[j++];
    }
    while (i <= m) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];

    for (int t = l; t <= r; ++t) a[t] = tmp[t];
}
