#include <iostream>
#include <string>
using namespace std;

int main() {
    string text;
    char letter;

    cout << "Въведете низ: ";
    getline(cin, text);
    cout << "Въведете буква: ";
    cin >> letter;

    if (!text.empty() && text[0] == letter)
        cout << "Низът започва с буквата '" << letter << "'." << endl;
    else
        cout << "Низът не започва с буквата '" << letter << "'." << endl;

    return 0;
}
