#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

class Rectangle {
private:
    double w, h;

public:
    // Конструктор
    Rectangle(double width, double height) { setWidth(width); setHeight(height); }

    // Getter-и
    double getWidth() const { return w; }
    double getHeight() const { return h; }

    // Setter-и с валидация
    void setWidth(double width) {
        if (width <= 0) throw invalid_argument("Ширината трябва да е положителна");
        w = width;
    }
    void setHeight(double height) {
        if (height <= 0) throw invalid_argument("Височината трябва да е положителна");
        h = height;
    }

    // Методи
    double area() const { return w * h; }
    double perimeter() const { return 2 * (w + h); }

    string to_string() const {
        return "Правоъгълник (" + std::to_string(w) + "x" + std::to_string(h) + 
               "), лице: " + std::to_string(area()) + 
               ", периметър: " + std::to_string(perimeter());
    }
};

// Демонстрация
int main() {
    try {
        Rectangle r(4.5, 3.2);
        cout << r.to_string() << "\n";
        r.setWidth(-1); // хвърля изключение
    } catch (const exception& e) {
        cerr << "Грешка: " << e.what() << "\n";
    }
    return 0;
}
