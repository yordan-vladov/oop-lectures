#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

class Student {
private:
    string name;
    string facultyNumber;
    double gpa;
    static int count;  // брояч на създадените обекти

public:
    // Конструктори
    Student(string n, string fn, double g)
        : name(move(n)), facultyNumber(move(fn)) {
        setGpa(g);
        ++count;
    }

    Student() : name(""), facultyNumber(""), gpa(0.0) { ++count; }

    // Деструктор
    ~Student() { --count; }

    // Getter-и
    string getName() const { return name; }
    string getFacultyNumber() const { return facultyNumber; }
    double getGpa() const { return gpa; }

    // Setter-и
    void setName(const string& n) { name = n; }
    void setFacultyNumber(const string& fn) { facultyNumber = fn; }
    void setGpa(double g) {
        if (g < 2.0 || g > 6.0)
            throw invalid_argument("Средният успех трябва да е между 2.00 и 6.00");
        gpa = g;
    }

    // Метод за текстово представяне
    string to_string() const {
        return "Студент: " + name + " (" + facultyNumber + 
               "), среден успех: " + std::to_string(gpa);
    }

    // Статичен метод за броя
    static int getCount() { return count; }
};

// Инициализация на статичния член
int Student::count = 0;

// Демонстрация
int main() {
    try {
        Student s1("Иван Петров", "F12345", 5.50);
        Student s2("Мария Георгиева", "F54321", 6.00);
        cout << s1.to_string() << "\n" << s2.to_string() << "\n";
        cout << "Брой създадени студенти: " << Student::getCount() << "\n";

        s2.setGpa(-2); // ще хвърли грешка
    } catch (const exception& e) {
        cerr << "Грешка: " << e.what() << "\n";
    }
    cout << "Оставащи обекти след края: " << Student::getCount() << "\n";
    return 0;
}
