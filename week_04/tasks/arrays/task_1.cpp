#include <iostream>
using namespace std;

int main() {
    const int n = 10;
    int arr[n];

    cout << "Въведете " << n << " числа:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    return 0;
}
