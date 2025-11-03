#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

class Book {
private:
    string title;
    string author;
    double price;

public:
    // Конструктор
    Book(string t, string a, double p) : title(move(t)), author(move(a)) {
        setPrice(p);
    }

    // Getter-и
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    double getPrice() const { return price; }

    // Setter-и
    void setTitle(const string& t) { title = t; }
    void setAuthor(const string& a) { author = a; }
    void setPrice(double p) {
        if (p < 0)
            throw invalid_argument("Цената не може да е отрицателна");
        price = p;
    }

    // Метод за описание на книгата
    string to_string() const {
        return "Книга: \"" + title + "\" от " + author +
               ", цена: " + std::to_string(price) + " лв.";
    }
};

// Демонстрация
int main_book() {
    try {
        Book b("Престъпление и наказание", "Ф.М. Достоевски", 14.99);
        cout << b.to_string() << "\n";
        b.setPrice(-5); // ще хвърли изключение
    } catch (const exception& e) {
        cerr << "Грешка: " << e.what() << "\n";
    }
    return 0;
}
