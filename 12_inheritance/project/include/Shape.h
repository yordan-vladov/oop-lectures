#pragma once
#include <iostream>

class Shape {
public:
    virtual ~Shape() = default;        // виртуален деструктор

    virtual double area() const = 0;   // чисто виртуална функция

    virtual void print() const {       // по избор – за демонстрация
        std::cout << "Shape\n";
    }
};
