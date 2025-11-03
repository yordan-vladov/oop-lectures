#include <iostream>
#include <limits>
using namespace std;

void clearBadInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readMenuChoice(int lo, int hi) {
    while (true) {
        cout << "Изберете [" << lo << "-" << hi << "]: ";
        int c;
        if (cin >> c && c >= lo && c <= hi) return c;
        cerr << "Невалиден избор. Опитайте пак.\n";
        clearBadInput();
    }
}

void submenuArithmetic() {
    while (true) {
        cout << "\n== Аритметика ==\n";
        cout << "1) Сумиране\n2) Изваждане\n3) Назад\n";
        int c = readMenuChoice(1, 3);
        if (c == 3) return;
        long long a, b;
        cout << "Въведете два цели операнда: ";
        while (!(cin >> a >> b)) { cerr << "Невалиден вход. "; clearBadInput(); }
        if (c == 1) cout << "Резултат: " << (a + b) << "\n";
        else        cout << "Резултат: " << (a - b) << "\n";
    }
}

void submenuInfo() {
    cout << "\n== Информация ==\nВерсия 1.0.0\nАвтор: Учебен пример\n\n";
}

int main() {
    while (true) {
        cout << "\n== Главно меню ==\n";
        cout << "1) Аритметика\n2) Информация\n3) Изход\n";
        int choice = readMenuChoice(1, 3);
        if (choice == 1) submenuArithmetic();
        else if (choice == 2) submenuInfo();
        else { cout << "Довиждане!\n"; break; }
    }
}
