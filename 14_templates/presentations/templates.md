# Шаблонни класове (Templates) в C++

---

## Какво представляват шаблоните?

* Механизъм на C++, който позволява:

  * създаване на **общоприложим код**, работещ с различни типове;
  * избягване на дублиране;
  * по-гъвкав дизайн на класове и функции.

---

* Основна идея:

  > Пишем **един код**, който работи с **различни типове**, определени при използването му.

---

## Шаблонни функции

```cpp
template <typename T>
T maxValue(T a, T b) {
    return (a > b) ? a : b;
}
```

* Извикване:

```cpp
int m1 = maxValue(3, 5);
double m2 = maxValue(3.14, 2.71);
```

---

## Шаблонни класове

```cpp
template <typename T>
class Box {
public:
    Box(T value) : value(value) {}

    T get() const { return value; }

private:
    T value;
};
```

---

* Използване:

```cpp
Box<int>    b1(10);
Box<double> b2(3.14);
Box<string> b3("Hello");
```

---

## Шаблони с няколко типа

```cpp
template <typename T, typename U>
class Pair {
public:
    Pair(T first, U second)
        : first(first), second(second) {}

    T getFirst() const  { return first; }
    U getSecond() const { return second; }

private:
    T first;
    U second;
};
```

---

## Имплементация в отделни файлове

⚠️ **Важно:**
Шаблоните работят само когато компилаторът вижда **цялата имплементация**.

Затова:

---
### Няма да работи добре:

* `Pair.h` → декларация
* `Pair.cpp` → имплементация
  (компилаторът няма да инстанцира шаблона за всички типове)
---
### Правилен подход:

* Всичко в **.h** файла
  или
* Използване на `.tpp` файл

---
## Пример: `.h` + `.tpp`

### Pair.h

```cpp
#pragma once
#include <iostream>

template <typename T, typename U>
class Pair {
public:
    Pair(T first, U second);

    T getFirst() const;
    U getSecond() const;

private:
    T first;
    U second;
};

#include "Pair.tpp"
```
---
### Pair.tpp

```cpp
template <typename T, typename U>
Pair<T, U>::Pair(T first, U second)
    : first(first), second(second) {}

template <typename T, typename U>
T Pair<T, U>::getFirst() const {
    return first;
}

template <typename T, typename U>
U Pair<T, U>::getSecond() const {
    return second;
}
```

---

## Частична и пълна специализация

### Пълна специализация

```cpp
template <>
class Box<bool> {
public:
    Box(bool value) : value(value) {}
    std::string get() const {
        return value ? "true" : "false";
    }
private:
    bool value;
};
```

---

### Частична специализация

```cpp
template <typename T>
class Box<T*> {
public:
    Box(T* ptr) : ptr(ptr) {}

    T getValue() const { return *ptr; }

private:
    T* ptr;
};
```

---

## Шаблони и наследяване

```cpp
template <typename T>
class Base {
public:
    void print(T value) { std::cout << value << "\n"; }
};

template <typename T>
class Derived : public Base<T> {
public:
    void printTwice(T value) {
        this->print(value);
        this->print(value);
    }
};
```

---

## Шаблони и полиморфизъм

⚠️ Шаблоните **не работят** с динамичен полиморфизъм (virtual),
а с **статична генерация** – компилаторът генерира нов клас за всеки тип.

---

# Обобщение: шаблоните в C++

* Позволяват създаване на **генеричен код**.
* Избягват дублирането.
* Работят чрез **статично инстанциране**.
* Имплементацията трябва да е **в header**, или да се използва `.tpp`.
---
* Поддържат:

  * специализация (пълна/частична);
  * шаблонни класове и функции;
  * шаблони с множество параметри.

---

# 4. **Задача: Шаблонен контейнер**

Създайте проект със следната структура:

```text
project/
├─ include/
│  ├─ Array.h
│  ├─ Array.tpp
├─ src/
│  ├─ main.cpp
├─ CMakeLists.txt
```

---

## Изисквания:

### 1. `Array<T>` – шаблонен клас

* съхранява масив от елементи `T`;
* съдържа:

  * конструктор `Array(size_t size)`;
  * метод `T& operator[](size_t index)` – достъп до елемент;
  * метод `size_t size() const`;
  * метод `void fill(const T& value)`.

Имплементацията е в **Array.tpp**.

---

## Примерно използване в `main.cpp`:

```cpp
#include "Array.h"
#include <iostream>

int main() {
    Array<int> a(5);
    a.fill(10);

    for (size_t i = 0; i < a.size(); ++i)
        std::cout << a[i] << " ";

    std::cout << "\n";

    Array<std::string> b(3);
    b.fill("Hi");

    for (size_t i = 0; i < b.size(); ++i)
        std::cout << b[i] << " ";

    return 0;
}
```

---

## CMake изисквания:

* компилира `main.cpp`;
* включва директория `include/`;
* **не компилира .tpp файла отделно** → той се включва в `Array.h`.