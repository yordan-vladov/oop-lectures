#include <iostream>
using namespace std;

int main()
{
    int n;
    cout << "Enter n: ";
    cin >> n;

    int positives = 0;
    int negatives = 0;
    int zeroes = 0;

    for (int i = 0; i < n; i++)
    {
        int number;
        cout << "Enter number: ";
        cin >> number;
        if (number > 0)
            positives++;
        else if (number < 0)
            negatives++;
        else
            zeroes++;
    }

    cout << "Positive numbers: " << positives << endl;
    cout << "Negative numbers: " << negatives << endl;
    cout << "Zeroes: " << zeroes << endl;

    return 0;
}