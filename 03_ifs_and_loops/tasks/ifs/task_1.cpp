#include <iostream>
using namespace std;

int main()
{
    int num;
    cout << "Enter number: ";
    cin >> num;

    if (num > 0)
        cout << num << " is positive." << endl;
    else if (num < 0)
        cout << num << " is negative." << endl;
    else
        cout << "Number is 0." << endl;
    
        return 0;
}