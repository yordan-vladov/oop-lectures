#include "MathUtils.h"
#include <iostream>

int main() {
    std::vector<int> nums = {5, 10, 15, 20};

    std::cout << "Sum: " << MathUtils::sum(nums) << '\n';
    std::cout << "Average: " << MathUtils::average(nums) << '\n';
    std::cout << "Max: " << MathUtils::max(nums) << '\n';

    return 0;
}
