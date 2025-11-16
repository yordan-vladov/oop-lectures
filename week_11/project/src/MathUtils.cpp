#include "MathUtils.h"
#include <numeric>  // std::accumulate
#include <algorithm> // std::max_element

int MathUtils::sum(const std::vector<int>& numbers) {
    return std::accumulate(numbers.begin(), numbers.end(), 0);
}

double MathUtils::average(const std::vector<int>& numbers) {
    if (numbers.empty()) return 0.0;
    return static_cast<double>(sum(numbers)) / numbers.size();
}

int MathUtils::max(const std::vector<int>& numbers) {
    if (numbers.empty()) return 0;
    return *std::max_element(numbers.begin(), numbers.end());
}
