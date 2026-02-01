#include <iostream>
using namespace std;

int sumArray(int arr[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++)
        sum += arr[i];
    return sum;
}

int main() {
    int n;
    cout << "Въведете броя на елементите: ";
    cin >> n;

    int arr[n];
    cout << "Въведете елементите:\n";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    cout << "Сумата на елементите е: " << sumArray(arr, n) << endl;
    return 0;
}
