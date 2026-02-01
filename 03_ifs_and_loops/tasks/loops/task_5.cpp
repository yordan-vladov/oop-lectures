#include <iostream>
using namespace std;

int main()
{
    int n;
    cout << "Enter n: ";
    cin >> n;

    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        int number;
        cout << "Enter number: ";
        cin >> number;

        if (i % 3 == 0)
            sum += number;
    }

    cout << sum << endl;
    return 0;
}