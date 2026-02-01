#include <iostream>
#include <vector>
#include <memory>

#include "Rectangle.h"
#include "Circle.h"

int main() {
    // Използваме smart pointers, за да не мислим за delete
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));
    shapes.push_back(std::make_unique<Circle>(2.0));
    shapes.push_back(std::make_unique<Rectangle>(5.0, 2.0));
    shapes.push_back(std::make_unique<Circle>(1.0));

    std::cout << "Shapes and their areas:\n";

    for (const auto& s : shapes) {
        s->print();
        std::cout << " -> area = " << s->area() << '\n';
    }

    return 0;
}
