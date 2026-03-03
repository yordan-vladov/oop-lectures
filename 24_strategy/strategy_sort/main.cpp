#include <iostream>
#include <vector>

#include "SorterContext.h"
#include "strategies/BubbleSort.h"
#include "strategies/QuickSort.h"
#include "strategies/MergeSort.h"
#include "strategies/HeapSort.h"

static void print(const std::vector<int>& v) {
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    std::vector<int> data { 7, 3, 9, 1, 6, 2, 8, 5, 4 };

    SorterContext ctx;

    std::cout << "Original: ";
    print(data);

    {
        auto v = data;
        ctx.setStrategy(std::make_unique<BubbleSort>());
        ctx.sort(v);
        std::cout << "BubbleSort: ";
        print(v);
    }

    {
        auto v = data;
        ctx.setStrategy(std::make_unique<QuickSort>());
        ctx.sort(v);
        std::cout << "QuickSort: ";
        print(v);
    }

    {
        auto v = data;
        ctx.setStrategy(std::make_unique<MergeSort>());
        ctx.sort(v);
        std::cout << "MergeSort: ";
        print(v);
    }

    // Additional challenge: new strategy without changing the context
    {
        auto v = data;
        ctx.setStrategy(std::make_unique<HeapSort>());
        ctx.sort(v);
        std::cout << "HeapSort: ";
        print(v);
    }

    return 0;
}
