#include <iostream>
using namespace std;

void swapNumbers(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x, y;
    cout << "Въведете две числа: ";
    cin >> x >> y;

    swapNumbers(&x, &y);

    cout << "След размяна: x = " << x << ", y = " << y << endl;

    return 0;
}
