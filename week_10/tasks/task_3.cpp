#include <iostream>
#include <string>

// Прост клас Student
class Student {
private:
    std::string name;
    int facultyNumber;

public:
    Student(const std::string& name = "", int fn = 0)
        : name(name), facultyNumber(fn) {}

    const std::string& getName() const { return name; }
    int getFacultyNumber() const { return facultyNumber; }

    void print() const {
        std::cout << "Student: " << name
                  << ", FN: " << facultyNumber << std::endl;
    }
};

class StudentList {
private:
    Student* students;
    std::size_t size;
    std::size_t capacity;

    void resize(std::size_t newCapacity) {
        Student* newData = new Student[newCapacity];
        for (std::size_t i = 0; i < size; ++i) {
            newData[i] = students[i];  // извиква копиращ оператор на Student
        }
        delete[] students;
        students = newData;
        capacity = newCapacity;
    }

public:
    // Конструктор по подразбиране
    StudentList()
        : students(nullptr), size(0), capacity(0) {}

    // Деструктор
    ~StudentList() {
        delete[] students;
    }

    // Копиращ конструктор
    StudentList(const StudentList& other)
        : students(nullptr), size(other.size), capacity(other.capacity)
    {
        if (capacity > 0) {
            students = new Student[capacity];
            for (std::size_t i = 0; i < size; ++i) {
                students[i] = other.students[i];
            }
        }
    }

    // Копиращ оператор=
    StudentList& operator=(const StudentList& other) {
        if (this != &other) {
            delete[] students;

            size = other.size;
            capacity = other.capacity;
            students = nullptr;

            if (capacity > 0) {
                students = new Student[capacity];
                for (std::size_t i = 0; i < size; ++i) {
                    students[i] = other.students[i];
                }
            }
        }
        return *this;
    }

    // Move конструктор
    StudentList(StudentList&& other) noexcept
        : students(other.students), size(other.size), capacity(other.capacity)
    {
        other.students = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    // Move оператор=
    StudentList& operator=(StudentList&& other) noexcept {
        if (this != &other) {
            delete[] students;

            students = other.students;
            size = other.size;
            capacity = other.capacity;

            other.students = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }

    // Добавяне на студент
    void add(const Student& s) {
        if (size == capacity) {
            std::size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            resize(newCapacity);
        }
        students[size++] = s;
    }

    // Премахване по фак. номер (връща true при успех)
    bool removeByFacultyNumber(int facultyNumber) {
        for (std::size_t i = 0; i < size; ++i) {
            if (students[i].getFacultyNumber() == facultyNumber) {
                // преместваме последния елемент на мястото на изтрития
                students[i] = students[size - 1];
                --size;
                return true;
            }
        }
        return false;
    }

    // Търсене по фак. номер (връща указател или nullptr)
    Student* findByFacultyNumber(int facultyNumber) {
        for (std::size_t i = 0; i < size; ++i) {
            if (students[i].getFacultyNumber() == facultyNumber) {
                return &students[i];
            }
        }
        return nullptr;
    }

    std::size_t getSize() const { return size; }

    void printAll() const {
        for (std::size_t i = 0; i < size; ++i) {
            students[i].print();
        }
    }
};

// Пример
int main() {
    StudentList list;

    list.add(Student("Ivan", 12345));
    list.add(Student("Maria", 23456));
    list.add(Student("Georgi", 34567));

    std::cout << "All students:" << std::endl;
    list.printAll();

    std::cout << "\nSearching for FN=23456:" << std::endl;
    Student* s = list.findByFacultyNumber(23456);
    if (s) {
        s->print();
    }

    std::cout << "\nRemoving FN=12345..." << std::endl;
    list.removeByFacultyNumber(12345);

    std::cout << "All students after remove:" << std::endl;
    list.printAll();
}
