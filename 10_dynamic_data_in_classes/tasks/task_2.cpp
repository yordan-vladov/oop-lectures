#include <iostream>
#include <cstring>

class Person {
private:
    char* name;

public:
    // Конструктор по подразбиране + от C-стринг
    Person(const char* n = "")
        : name(nullptr)
    {
        name = new char[std::strlen(n) + 1];
        std::strcpy(name, n);
    }

    // Деструктор
    ~Person() {
        delete[] name;
    }

    // Копиращ конструктор
    Person(const Person& other)
        : name(nullptr)
    {
        if (other.name) {
            name = new char[std::strlen(other.name) + 1];
            std::strcpy(name, other.name);
        }
    }

    // Копиращ оператор=
    Person& operator=(const Person& other) {
        if (this != &other) {
            delete[] name;

            if (other.name) {
                name = new char[std::strlen(other.name) + 1];
                std::strcpy(name, other.name);
            } else {
                name = nullptr;
            }
        }
        return *this;
    }

    // Move конструктор
    Person(Person&& other) noexcept
        : name(other.name)
    {
        other.name = nullptr;
    }

    // Move оператор=
    Person& operator=(Person&& other) noexcept {
        if (this != &other) {
            delete[] name;

            name = other.name;
            other.name = nullptr;
        }
        return *this;
    }

    void print() const {
        std::cout << "Name: " << (name ? name : "(null)") << std::endl;
    }
};

// Пример
int main() {
    Person p1("Ivan");
    Person p2 = p1;            // копиране
    Person p3 = Person("Maria"); // move (RVO / move)

    p1.print();
    p2.print();
    p3.print();

    Person p4;
    p4 = std::move(p3);        // move оператор=
    p4.print();
}
