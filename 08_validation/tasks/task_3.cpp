#include <iostream>
#include <limits>
using namespace std;

static void clearBad() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt() {
    int x;
    while (true) {
        if (cin >> x) { clearBad(); return x; } // чистим \n след успешен прочит
        cerr << "Невалидно цяло число. Опитайте пак: ";
        clearBad();
    }
}

int readIntInRange(int lo, int hi) {
    while (true) {
        cout << "Въведете цяло число в диапазон [" << lo << ", " << hi << "]: ";
        int v = readInt();
        if (v >= lo && v <= hi) return v;
        cerr << "Стойността трябва да е между " << lo << " и " << hi << ".\n";
    }
}

// демо: въвеждане на възраст
int main_demo_age() {
    cout << "Възраст: ";
    int age = readIntInRange(0, 130);
    cout << "Въведохте възраст: " << age << "\n";
    return 0;
}
