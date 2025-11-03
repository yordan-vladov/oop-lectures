#include <iostream>
#include <string>
using namespace std;

int main() {
    string text;
    cout << "Въведете низ: ";
    getline(cin, text);

    cout << "Дължината на низа е: " << text.length() << endl;

    return 0;
}
