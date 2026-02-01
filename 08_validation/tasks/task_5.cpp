#include <iostream>
#include <vector>
#include <limits>
using namespace std;

static void clearBad2() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readIntSafePrompt(const string& prompt) {
    while (true) {
        cout << prompt;
        int v;
        if (cin >> v) { clearBad2(); return v; }
        cerr << "Невалиден вход. Опитайте отново.\n";
        clearBad2();
    }
}

int main_numbers_list() {
    int N = readIntSafePrompt("Въведете N: ");
    while (N < 0) {
        cerr << "N трябва да е неотрицателно.\n";
        N = readIntSafePrompt("Въведете N: ");
    }

    vector<int> a(N);
    for (int i = 0; i < N; ++i) {
        a[i] = readIntSafePrompt("Елемент[" + to_string(i) + "]: ");
    }

    cout << "Въведени " << N << " елемента:\n";
    for (int i = 0; i < N; ++i) cout << a[i] << (i + 1 == N ? '\n' : ' ');
    return 0;
}
