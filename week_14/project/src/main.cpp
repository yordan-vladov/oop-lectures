#include <iostream>
#include <string>
#include "Array.h"

int main() {
    try {
        // Пример с int
        Array<int> a(5);
        a.fill(10);

        std::cout << "Array<int>: ";
        for (std::size_t i = 0; i < a.size(); ++i) {
            std::cout << a[i] << " ";
        }
        std::cout << "\n";

        // Пример със std::string
        Array<std::string> b(3);
        b.fill("Hi");

        std::cout << "Array<string>: ";
        for (std::size_t i = 0; i < b.size(); ++i) {
            std::cout << b[i] << " ";
        }
        std::cout << "\n";

        // Демонстрация на копиране
        Array<std::string> c = b; // копиращ конструктор
        c[1] = "Hello";

        std::cout << "Copied & modified Array<string>: ";
        for (std::size_t i = 0; i < c.size(); ++i) {
            std::cout << c[i] << " ";
        }
        std::cout << "\n";

    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
    }

    return 0;
}
