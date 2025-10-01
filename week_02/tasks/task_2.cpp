#include <iostream>
using namespace std;

int main()
{
    // int a,b;
    double a, b;

    cout << "Enter side a: ";
    cin >> a;

    cout << "Enter side b: ";
    cin >> b;

    cout << "Perimeter: " << 2 * (a + b) << endl;
    cout << "Area: " << a * b << endl;

    return 0;
}