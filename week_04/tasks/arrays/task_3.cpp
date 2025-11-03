#include <iostream>
using namespace std;

int main() {
    const int n = 10;
    int arr[n];

    cout << "Въведете " << n << " числа:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int min = arr[0];
    int max = arr[0];

    for (int i = 1; i < n; i++) {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
    }

    cout << "Най-малко число: " << min << endl;
    cout << "Най-голямо число: " << max << endl;

    return 0;
}
