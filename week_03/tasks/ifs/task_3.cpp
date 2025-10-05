#include <iostream>
using namespace std;

int main()
{
    int a, b, c;

    cout << "Enter a: ";
    cin >> a;

    cout << "Enter b: ";
    cin >> b;

    cout << "Enter c: ";
    cin >> c;

    if (a >= b && a >= c)
        cout << a << " is largest.";
    else if (b >= a && b >= c)
        cout << b << " is largest.";
    else
        cout << c << " is largest.";

    cout << endl;

    return 0;
}