#include <iostream>
using namespace std;

int main() {
    const int N = 3;
    int matrix[N][N];
    int sumDiagonal = 0;

    cout << "Въведете елементите на 3x3 матрица:\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> matrix[i][j];
            if (i == j) {
                sumDiagonal += matrix[i][j]; // главен диагонал
            }
        }
    }

    cout << "Сумата по главния диагонал е: " << sumDiagonal << endl;

    return 0;
}
