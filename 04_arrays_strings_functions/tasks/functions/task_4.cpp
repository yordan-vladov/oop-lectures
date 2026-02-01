#include <iostream>
#include <string>
using namespace std;

int countVowels(const string& text) {
    string vowels = "aeiouAEIOUауеоиюяАУЕОИЮЯ";
    int count = 0;
    for (char c : text)
        if (vowels.find(c) != string::npos)
            count++;
    return count;
}

int main() {
    string s;
    cout << "Въведете текст: ";
    getline(cin, s);

    cout << "Брой гласни: " << countVowels(s) << endl;
    return 0;
}
