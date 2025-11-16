#include <iostream>

class DynamicArray {
private:
    int* data;
    std::size_t size;
    std::size_t capacity;

    void resize(std::size_t newCapacity) {
        int* newData = new int[newCapacity];
        for (std::size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    // Конструктор по подразбиране
    DynamicArray()
        : data(nullptr), size(0), capacity(0) {}

    // Деструктор
    ~DynamicArray() {
        delete[] data;
    }

    // Копиращ конструктор
    DynamicArray(const DynamicArray& other)
        : data(nullptr), size(other.size), capacity(other.capacity) {
        if (capacity > 0) {
            data = new int[capacity];
            for (std::size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
    }

    // Копиращ оператор=
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;

            size = other.size;
            capacity = other.capacity;
            data = nullptr;

            if (capacity > 0) {
                data = new int[capacity];
                for (std::size_t i = 0; i < size; ++i) {
                    data[i] = other.data[i];
                }
            }
        }
        return *this;
    }

    // Move конструктор
    DynamicArray(DynamicArray&& other) noexcept
        : data(other.data), size(other.size), capacity(other.capacity) {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    // Move оператор=
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] data;

            data = other.data;
            size = other.size;
            capacity = other.capacity;

            other.data = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }

    // Добавяне на елемент
    void push_back(int value) {
        if (size == capacity) {
            std::size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            resize(newCapacity);
        }
        data[size++] = value;
    }

    // Размер
    std::size_t getSize() const {
        return size;
    }

    // Достъп до елемент
    int& operator[](std::size_t index) {
        return data[index]; // без проверки за простота
    }

    const int& operator[](std::size_t index) const {
        return data[index];
    }
};

// Малък пример
int main() {
    DynamicArray arr;
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);

    for (std::size_t i = 0; i < arr.getSize(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}
