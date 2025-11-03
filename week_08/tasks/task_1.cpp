#include <iostream>
#include <string>
#include <sstream>
using namespace std;

double readDouble() {
    while (true) {
        cout << "Въведете реално число: ";
        string line; 
        if (!getline(cin, line)) throw runtime_error("Входът приключи неочаквано");
        stringstream ss(line);
        double v; char extra;
        if (ss >> v && !(ss >> extra)) return v;     // няма излишни символи
        cerr << "Невалиден формат. Пример: 3.14, -2, 0\n";
    }
}
