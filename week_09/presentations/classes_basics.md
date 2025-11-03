# Класове в C++ – Преговор и добри практики

---
## Съдържание

* Какво е клас – напомняне
* Конструктори и деструктори
* Getters и Setters
* `to_string()` и представяне на обекти
---
* Правила на трите/петте (Rule of 3/5)
* Статични членове и константни методи
* Пример: клас `Car`
* Задачи
---
# Какво е клас?

---

* **Класът** е **потребителски тип**, който обединява:
  * **данни (член-променливи)**
  * **поведение (методи)**
---
```cpp
class Car
{
private:
    string brand;
    int year;
    double price;

public:
    // Методи и конструктори тук
};
```
---
# Конструктори

---

* Използват се за **инициализация на обекти**.
* Имат същото име като класа.
* Могат да бъдат **по подразбиране**, **параметризирани**, **копиращи** или **преместващи**.
---
```cpp
class Car
{
    string brand;
    int year{0};
    double price{0};

public:
    Car() = default; // конструктор по подразбиране
    Car(string b, int y, double p) : brand(b), year(y), price(p) {}
};
```

---
### Списък за инициализация

* По-ефективен начин за инициализация на членове.

```cpp
Car::Car(string b, int y, double p)
    : brand(b), year(y), price(p) {}
```
---
# Деструктори

---

* **Автоматично се извикват** при излизане на обекта от обхват.
* Освобождават **ресурси** (динамична памет, файлове, връзки и др.).
---
```cpp
class Car
{
public:
    ~Car() { cout << "Car destroyed\n"; }
};
```
---
# Getters и Setters

---
* Осигуряват **контролиран достъп** до частните данни.

```cpp
class Car
{
private:
    string brand;
    double price;

public:
    void setPrice(double p) {
        if (p >= 0) price = p;
        else throw invalid_argument("Negative price");
    }

    double getPrice() const { return price; }
    string getBrand() const { return brand; }
};
```
---
# `to_string()` метод

---
* Удобен за **текстово представяне** на обект (например за `cout`).

```cpp
class Car
{
    string brand;
    int year;
    double price;

public:
    string to_string() const
    {
        return brand + " (" + std::to_string(year) + ") - " + std::to_string(price) + " лв.";
    }
};
```
---
```cpp
Car c{"Toyota", 2020, 25500};
cout << c.to_string() << endl;
```

---

# Пример: Пълен клас `Car`

---

```cpp
#include <iostream>
#include <string>
using namespace std;

class Car
{
private:
    string brand;
    int year;
    double price;

public:
    // Конструктори
    Car() : brand("Unknown"), year(2000), price(0) {}
    Car(string b, int y, double p) : brand(b), year(y), price(p) {}

    // Getter-и и Setter-и
    string getBrand() const { return brand; }
    int getYear() const { return year; }
    double getPrice() const { return price; }

    void setPrice(double p) {
        if (p >= 0) price = p;
    }

    // Метод за представяне
    string to_string() const {
        return brand + " (" + std::to_string(year) + ") - " + std::to_string(price) + " лв.";
    }

    // Деструктор
    ~Car() { cout << "Destroying " << brand << endl; }
};

int main()
{
    Car c1{"Honda", 2018, 18000};
    cout << c1.to_string() << endl;

    c1.setPrice(19500);
    cout << "New price: " << c1.getPrice() << endl;
}
```

---
# Rule of 3 / Rule of 5

---

| №   | Метод                   | Значение               |
| --- | ----------------------- | ---------------------- |
| 1   | Копиращ конструктор     | създава копие на обект |
| 2   | Копиращ оператор `=`    | копира стойности       |
| 3   | Деструктор              | освобождава ресурси    |
| 4   | Преместващ конструктор  | прехвърля ресурси      |
| 5   | Преместващ оператор `=` | прехвърля ресурси      |

---

> Ако дефинирате **един** от следните методи, вероятно трябва да дефинирате **всички**.

---
# Статични членове

---

* **Общи за всички обекти** на класа.
* Полезни за **броене на инстанции**, **споделени настройки** и др.

```cpp
class Car
{
    static int total;
public:
    Car() { ++total; }
    ~Car() { --total; }

    static int getTotal() { return total; }
};

int Car::total = 0;
```

---
# Константни методи

---

* **Не променят** състоянието на обекта.
* Могат да се извикват и върху **const обекти**.

```cpp
class Student
{
    string name;
public:
    string getName() const { return name; } // const метод
};
```

---
# Добри практики

---

* Използвайте **`explicit`** за конструктори с един аргумент.
* Използвайте **инициализация** вместо присвояване в тялото.
* Винаги спазвайте **const-коректност**.
* Използвайте **`= default`** и **`= delete`** при нужда.
* Избягвайте “голи” указатели – предпочитайте `std::vector`, `std::unique_ptr`, `std::string`.

---
# Задачи

---

1. Имплементирайте клас `Book` с:
   * членове: `title`, `author`, `price`;
   * `to_string()` метод;
   * setter с проверка за неотрицателна цена.
---
2. Направете клас `Rectangle`, който има:

   * членове `w`, `h`;
   * методи `area()` и `perimeter()`;
   * getter/setter с валидация.
---
3. Реализирайте клас `Student` с:
   * име, факултетен номер, среден успех;
   * конструктори, getter-и и `to_string()`;
   * брояч на всички създадени обекти (`static` член).

---
# Допълнително (по желание)

---

* Оператори `<<` и `>>` за вход/изход
* Оператори за сравнение (`==`, `<=>`)
* Композиция и наследяване
* Конструктори за преместване (`move semantics`)
