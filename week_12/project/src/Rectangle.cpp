#include "Rectangle.h"
#include <iostream>

Rectangle::Rectangle(double width, double height)
    : width(width), height(height) {}

double Rectangle::area() const {
    return width * height;
}

void Rectangle::print() const {
    std::cout << "Rectangle ("
              << width << " x " << height << ")";
}
