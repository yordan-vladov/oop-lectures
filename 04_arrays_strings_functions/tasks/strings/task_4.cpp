#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    string text;
    cout << "Въведете низ: ";
    getline(cin, text);

    reverse(text.begin(), text.end());

    cout << "Обърнат низ: " << text << endl;

    return 0;
}
