#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
using namespace std;

bool isLeap(int y) {
    return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

int daysInMonth(int m, int y) {
    switch (m) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
        case 4: case 6: case 9: case 11: return 30;
        case 2: return isLeap(y) ? 29 : 28;
        default: return 0;
    }
}

struct Date { int d, m, y; };

Date parseDate(const string& s) {
    // формат: DD.MM.YYYY (без доп. символи)
    int d, m, y; char dot1, dot2;
    stringstream ss(s);
    if (!(ss >> d >> dot1 >> m >> dot2 >> y) || (dot1 != '.' || dot2 != '.') || ss.peek() != EOF) {
        throw invalid_argument("Невалиден формат (очаква се DD.MM.YYYY)");
    }
    if (y < 1900 || y > 2100) throw out_of_range("Годината е извън [1900,2100]");
    if (m < 1 || m > 12) throw out_of_range("Месецът е извън [1,12]");
    int dim = daysInMonth(m, y);
    if (d < 1 || d > dim) throw out_of_range("Денят е извън допустимия диапазон за месеца");
    return {d, m, y};
}

int main() {
    cout << "Въведете дата (DD.MM.YYYY): ";
    string s;
    if (!getline(cin, s)) return 0;

    try {
        Date dt = parseDate(s);
        cout << "OK: " << dt.d << "." << dt.m << "." << dt.y << "\n";
    } catch (const exception& e) {
        cerr << "Грешка: " << e.what() << "\n";
    }
}
