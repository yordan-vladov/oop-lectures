#include <iostream>
using namespace std;

int main() {
    const int n = 10;
    int arr[n];
    int sum = 0;

    cout << "Въведете " << n << " числа:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        sum += arr[i];
    }

    double average = (double)sum / n;

    cout << "Сумата на елементите е: " << sum << endl;
    cout << "Средната стойност е: " << average << endl;

    return 0;
}
