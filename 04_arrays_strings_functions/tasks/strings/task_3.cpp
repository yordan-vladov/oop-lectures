#include <iostream>
#include <string>
using namespace std;

int main() {
    string sentence, word;

    cout << "Въведете изречение: ";
    getline(cin, sentence);
    cout << "Въведете дума за търсене: ";
    getline(cin, word);

    size_t pos = sentence.find(word);

    if (pos != string::npos)
        cout << "Думата \"" << word << "\" е намерена на позиция: " << pos << endl;
    else
        cout << "Думата \"" << word << "\" не е намерена." << endl;

    return 0;
}
