#include <stdexcept> // std::out_of_range
#include "Array.h"

template <typename T>
Array<T>::Array(std::size_t size)
    : data(nullptr), m_size(size)
{
    if (m_size == 0) {
        data = nullptr;
    } else {
        data = new T[m_size]{}; // value-initialize
    }
}

template <typename T>
Array<T>::Array(const Array& other)
    : data(nullptr), m_size(0)
{
    copyFrom(other);
}

template <typename T>
Array<T>& Array<T>::operator=(const Array& other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

template <typename T>
Array<T>::~Array() {
    free();
}

template <typename T>
void Array<T>::copyFrom(const Array& other) {
    m_size = other.m_size;
    if (m_size == 0) {
        data = nullptr;
        return;
    }

    data = new T[m_size];
    for (std::size_t i = 0; i < m_size; ++i) {
        data[i] = other.data[i];
    }
}

template <typename T>
void Array<T>::free() {
    delete[] data;
    data = nullptr;
    m_size = 0;
}

template <typename T>
T& Array<T>::operator[](std::size_t index) {
    if (index >= m_size) {
        throw std::out_of_range("Array index out of range");
    }
    return data[index];
}

template <typename T>
const T& Array<T>::operator[](std::size_t index) const {
    if (index >= m_size) {
        throw std::out_of_range("Array index out of range");
    }
    return data[index];
}

template <typename T>
std::size_t Array<T>::size() const {
    return m_size;
}

template <typename T>
void Array<T>::fill(const T& value) {
    for (std::size_t i = 0; i < m_size; ++i) {
        data[i] = value;
    }
}
