#include <iostream>
#include <stdexcept>
#include <vector>
#include <climits>
using namespace std;

// безопасно целочислено деление:
// - хвърля invalid_argument при деление на нула
// - хвърля overflow_error при INT_MIN / -1 (препълване за 2's complement)
int safe_div(int a, int b) {
    if (b == 0) throw invalid_argument("Деление на нула");
    if (a == INT_MIN && b == -1) throw overflow_error("Препълване при деление (INT_MIN / -1)");
    return a / b;
}

int main() {
    vector<pair<int,int>> tests = {
        {10, 2}, {9, -3}, {-9, 3}, {INT_MIN, -1}, {5, 0}, {INT_MIN, 1}, {INT_MAX, -1}
    };

    for (auto [a, b] : tests) {
        try {
            int r = safe_div(a, b);
            cout << a << " / " << b << " = " << r << "\n";
        } catch (const exception& e) {
            cout << a << " / " << b << " -> Грешка: " << e.what() << "\n";
        }
    }
}
