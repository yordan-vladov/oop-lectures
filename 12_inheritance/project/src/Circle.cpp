#include "Circle.h"
#include <iostream>

Circle::Circle(double radius)
    : radius(radius) {}

double Circle::area() const {
    const double pi = 3.141592653589793;
    return pi * radius * radius;
}

void Circle::print() const {
    std::cout << "Circle (r = " << radius << ")";
}
