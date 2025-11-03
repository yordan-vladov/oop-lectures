#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Въведете n: ";
    cin >> n;

    int* arr = new int[n]; // динамичен масив
    int sum = 0;

    cout << "Въведете " << n << " числа:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        sum += arr[i];
    }

    double average = (double)sum / n;

    cout << "Средната стойност е: " << average << endl;

    delete[] arr; // освобождаваме паметта
    return 0;
}
