#include <iostream>
using namespace std;

int main()
{
    int grade;
    cout << "Enter grade: ";
    cin >> grade;

    switch (grade)
    {
    case 2:
        cout << "Very weak" << endl;
        break;

    case 3:
        cout << "Weak" << endl;
        break;

    case 4:
        cout << "Decent" << endl;
        break;

    case 5:
        cout << "Good" << endl;
        break;

    case 6:
        cout << "Excellent" << endl;
        break;
    default:
        cout << "Invalid grade" << endl;
        break;
    }
    return 0;
}