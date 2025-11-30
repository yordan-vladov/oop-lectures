#pragma once
#include <cstddef>   // size_t

template <typename T>
class Array {
public:
    // Конструктор с размер
    explicit Array(std::size_t size);

    // Копиращ конструктор
    Array(const Array& other);

    // Копиращо присвояване
    Array& operator=(const Array& other);

    // Деструктор
    ~Array();

    // Достъп до елемент (за промяна)
    T& operator[](std::size_t index);

    // Достъп до елемент (за константни обекти)
    const T& operator[](std::size_t index) const;

    // Връща броя елементи
    std::size_t size() const;

    // Попълва целия масив с дадена стойност
    void fill(const T& value);

private:
    T* data;
    std::size_t m_size;

    void copyFrom(const Array& other);
    void free();
};

// Имплементацията на шаблона
#include "Array.tpp"
