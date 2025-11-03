#include <iostream>
#include <limits>
#include <vector>
using namespace std;

void clearBadInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt() {
    int x;
    while (true) {
        if (cin >> x) return x;
        cerr << "Невалиден вход. Опитайте пак: ";
        clearBadInput();
    }
}

int readIntInRange(int lo, int hi) {
    while (true) {
        int v = readInt();
        if (v >= lo && v <= hi) return v;
        cerr << "Стойността трябва да е в диапазон [" << lo << ", " << hi << "]. Опитайте пак: ";
    }
}

int main() {
    cout << "Брой оценки N: ";
    int N = readIntInRange(1, 100000); // лимит по избор

    vector<int> grades;
    grades.reserve(N);

    cout << "Въведете " << N << " оценки (0..100):\n";
    for (int i = 0; i < N; ++i) {
        cout << "Оценка[" << i << "]: ";
        grades.push_back(readIntInRange(0, 100));
    }

    long long sum = 0;
    for (int g : grades) sum += g;
    double avg = grades.empty() ? 0.0 : (double)sum / grades.size();

    cout.setf(ios::fixed); cout.precision(2);
    cout << "Среден успех: " << avg << "\n";
}
