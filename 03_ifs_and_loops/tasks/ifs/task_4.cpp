#include <iostream>
using namespace std;

int main()
{
    char c;
    cout << "Enter symbol: ";
    cin >> c;

    char lower_c = tolower(c);

    if (lower_c == 'a' || lower_c == 'e' ||
        lower_c == 'i' || lower_c == 'o' || lower_c == 'u')
        cout << c << " is a vowel." << endl;
    else if (lower_c >= 'a' && lower_c <= 'z')
        cout << c << " is a consonant." << endl;   
    else 
        cout << c << " is not a letter." << endl;

    
    return 0;
}