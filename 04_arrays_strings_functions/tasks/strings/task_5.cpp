#include <iostream>
#include <string>
using namespace std;

int main() {
    string text;
    cout << "Въведете низ: ";
    getline(cin, text);

    int count = 0;
    string vowels = "aeiouAEIOUауеоиюяАУЕОИЮЯ";

    for (char c : text) {
        if (vowels.find(c) != string::npos)
            count++;
    }

    cout << "Брой на гласните букви: " << count << endl;

    return 0;
}
