#include <iostream>
using namespace std;

int* createSquares(int N) {
    int* arr = new int[N];
    for (int i = 0; i < N; i++) {
        arr[i] = (i + 1) * (i + 1);
    }
    return arr; // връща пойнтър към динамичния масив
}

int main() {
    int N;
    cout << "Въведете N: ";
    cin >> N;

    int* squares = createSquares(N);

    cout << "Квадратите на първите " << N << " числа са:\n";
    for (int i = 0; i < N; i++)
        cout << squares[i] << " ";

    cout << endl;
    delete[] squares; // освобождаваме паметта

    return 0;
}
