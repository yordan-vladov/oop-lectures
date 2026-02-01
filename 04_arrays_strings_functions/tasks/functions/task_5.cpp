#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

string reverseString(string text) {
    reverse(text.begin(), text.end());
    return text;
}

int main() {
    string s;
    cout << "Въведете низ: ";
    getline(cin, s);

    cout << "Обърнат низ: " << reverseString(s) << endl;
    return 0;
}
