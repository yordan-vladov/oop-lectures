# Структури, класове и референтни типове

---
## Съдържание

* Структури (`struct`) – декларация, инициализация, достъп
* Класове (`class`) – инкапсулация, конструктори, методи
* `struct` vs `class`
* Референтни типове – `&`, `const &`, `&&` и кога се ползват
* Примери и добри практики
* Задачи
---
# Структури (`struct`)

---
### Какво е структура?

* **Структура** е **потребителски тип**, който групира логически свързани данни (и по избор – функции).
* Полезна за представяне на "записи" (record): студент, точка, дата и др.
---
```cpp
struct Point
{
    double x; // член-данни
    double y;
};

int main()
{
    Point p;   // декларация
    p.x = 3.5; // достъп с точка
    p.y = -2.0;
}
```

---
### Инициализация на структура

```cpp
Point a{1.0, 2.0};     // списък за инициализация (aggregate init)
Point b = {3.0, 4.0};  // еквивалентно

// Масив от структури
Point polyline[3] = { {0,0}, {1,1}, {2,1} };
```
* **Aggregate initialization** – налична, ако няма потребителски дефинирани конструктори и всички членове са публични.
---
### Структури и функции

```cpp
#include <iostream>
using namespace std;

struct Student
{
    string name;
    int fn;
    double grade;
};

void print(const Student &s)
{ // const ref – не копира и не променя
    cout << s.name << " (" << s.fn << ") → " << s.grade << "\n";
}

void curve(Student &s, double delta)
{ // ref – позволява промяна
    s.grade += delta;
}

int main()
{
    Student st{"Ana", 12345, 5.25};
    print(st);
    curve(st, 0.25);
    print(st);
}
```
---
### Вложени структури и композиция

```cpp
struct Date { int d, m, y; };
struct Person 
{
    string name;
    Date   born;
};

Person p{"Ivan", {28, 10, 2004}}; // достъп: p.born.y
```
---
# Класове (`class`)
---
### Защо класове?

* Добавят **инкапсулация** (скриване на данни), **инварианти**, **контрол на инициализацията** и поведение чрез **методи**.

```cpp
class BankAccount
{
private:
    string owner;
    double balance{0.0};

public:
    // Конструктори
    BankAccount(string owner, double initial)
        : owner(owner), balance(initial) {}

    // Методи (член-функции)
    void deposit(double amount) { balance += amount; }
    bool withdraw(double amount)
    {
        if (amount > balance)
            return false;
        balance -= amount;
        return true;
    }

    double getBalance() const { return balance; } // const-метод
};

int main()
{
    BankAccount acc{"Mila", 100.0};
    acc.deposit(50);
    acc.withdraw(20);
}
```
---
### Списък за инициализация и `this`

* Списъкът след двоеточието инициализира директно членовете.
* В методите указателят `this` сочи към текущия обект.

```cpp
class Vec2
{
    double x, y;

public:
    Vec2(double x, double y) : x(x), y(y) {}
    Vec2 &add(const Vec2 &other)
    {
        this->x += other.x; // еквивалентно на: x += other.x;
        this->y += other.y;
        return *this; // позволява верига: a.add(b).add(c)
    }
};
```

---

### `struct` vs `class`

| Характеристика              | `struct` (по подразбиране) | `class` (по подразбиране) |
| --------------------------- | -------------------------- | ------------------------- |
| Достъпност по подразбиране  | `public`                   | `private`                 |
| Наследяване по подразбиране | публично                   | частно                    |
| Употреба (конвенция)        | пасивни данни/POCO         | инкапсулирани типове      |

> **Забележка:** И `struct`, и `class` могат да имат конструктори, методи, статични членове, оператори и т.н.

---

### Конструктори: default, copy, move

```cpp
class Box 
{
    int w{0}, h{0};
public:
    Box() = default;                  // default конструктор
    Box(int w, int h): w(w), h(h) {}  // потребителски

    // copy/move (генерират се автоматично при нужда)
    Box(const Box&) = default;        // копиращ
    Box& operator=(const Box&) = default;
    Box(Box&&) = default;             // преместване
    Box& operator=(Box&&) = default;
};
```

* Може да **забранявате** операции: `= delete;`
---
```cpp
#include <iostream>
using namespace std;

class Box {
    int w{0}, h{0};

public:
    // 1️⃣ Default constructor
    Box() {
        cout << "Default constructor called\n";
    }

    // 2️⃣ Parameterized (user-defined) constructor
    Box(int w, int h) : w(w), h(h) {
        cout << "Parameterized constructor called\n";
    }

    // 3️⃣ Copy constructor
    Box(const Box& other) : w(other.w), h(other.h) {
        cout << "Copy constructor called\n";
    }

    // 4️⃣ Copy assignment
    Box& operator=(const Box& other) {
        cout << "Copy assignment operator called\n";
        if (this != &other) { // prevent self-assignment
            w = other.w;
            h = other.h;
        }
        return *this;
    }

    // 5️⃣ Move constructor
    Box(Box&& other) noexcept : w(other.w), h(other.h) {
        cout << "Move constructor called\n";
        other.w = 0; // optional: "empty" the source
        other.h = 0;
    }

    // 6️⃣ Move assignment
    Box& operator=(Box&& other) noexcept {
        cout << "Move assignment operator called\n";
        if (this != &other) {
            w = other.w;
            h = other.h;
            other.w = 0;
            other.h = 0;
        }
        return *this;
    }

    ~Box() {
        cout << "Destructor called\n";
    }
};

// Helper function to produce a temporary Box
Box makeBox() {
    Box temp(10, 20);  // parameterized constructor
    return temp;       // triggers move constructor (usually)
}

int main() {
    cout << "--- b1 ---\n";
    Box b1;                        // Default constructor

    cout << "--- b2 ---\n";
    Box b2(3, 4);                  // Parameterized constructor

    cout << "--- b3 (copy) ---\n";
    Box b3 = b2;                   // Copy constructor

    cout << "--- b4 (copy assignment) ---\n";
    b1 = b2;                       // Copy assignment

    cout << "--- b5 (move) ---\n";
    Box b5 = makeBox();            // Move constructor (usually)

    cout << "--- b6 (move assignment) ---\n";
    b1 = makeBox();                // Move assignment (usually)

    cout << "--- End of main ---\n";
    return 0;
}
```
---

### Константност при методи

```cpp
class Image 
{
    int width{0};
public:
    int getWidth() const { return width; }
    void setWidth(int w) { width = w; }
};

const Image img;   // само const-методи са достъпни
img.getWidth();    // ОК
// img.setWidth(10); // ГРЕШКА
```

---
### Дескруктори

- **Деструкторът** се извиква **автоматично**, когато обектът излезе от обхват или се изтрие (`delete`).
- Използва се за **освобождаване на ресурси** – памет, файлове, сокети и др.
---
```cpp
#include <iostream>
using namespace std;

class Box 
{
public:
    // Constructor
    Box() 
    {
        cout << "Constructor called\n";
    }

    // Destructor
    ~Box() 
    {
        cout << "Destructor called\n";
    }
};

int main() 
{
    {
        Box b;  // constructor runs here
        cout << "Inside the block\n";
    }           // b goes out of scope → destructor runs here

    cout << "After the block\n";
    return 0;
}
```
---
```cpp
class File
{
    FILE *f;

public:
    File(const char *name, const char *mode)
    {
        f = fopen(name, mode);
    }
    ~File()
    {
        if (f)
            fclose(f); // автоматично затваряне на файла
    }
};

int main()
{
    File data("info.txt", "w");
    // файлът ще се затвори автоматично при изход от обхвата
}
```
---
### Статични членове

```cpp
class Counter 
{
public:
    static int live;
    Counter() { ++live; }
    ~Counter(){ --live; }
};

int Counter::live = 0; // дефиниция
```

* Достъп: `Counter::live` (без инстанция).

---
# Референтни типове

---

### Lvalue референции `T&`

* Дават **алтернативно име** на съществуващ обект.
* Позволяват модификация на оригинала.

```cpp
void twice(int& x) { x *= 2; }
```

---

### Константни референции `const T&`

* **Не копират** аргумента и **не позволяват промяна**.
* Полезни за големи обекти и за приемане на временни обекти (rvalues).

```cpp
void printVec(const vector<int>& v) 
{
    for(int x : v) cout << x << ' ';
}
```

---

### Кога как да подаваме параметри?

| Случай                                | Препоръка                    |
| ------------------------------------- | ---------------------------- |
| Малки типове (int, double, pointers)  | По стойност                  |
| Големи обекти, без промяна            | `const T&`                   |
| Големи обекти, ще се задържат/копират | `T` и използвайте move вътре |
| Трябва да променим аргумента          | `T&`                         |

---
# Добри практики

---

* Поддържайте инварианти чрез **private** данни и **public** интерфейс.
* Инициализирайте в **списък за инициализация**; избягвайте празни конструктори + присвояване.
* Предпочитайте **`const` коректност**.
* Ползвайте **`= default`** и **`= delete`** вместо празни тела.
---
# Примери
---
### Компактен `struct` с методи

```cpp
struct Rect 
{
    int w{0}, h{0};
    int area() const { return w * h; }
};

Rect r{3,4};
cout << r.area(); // 12
```
---
### Клас с инкапсулация и валидация

```cpp
class Temperature 
{
    double celsius{0};
public:
    explicit Temperature(double c){ setC(c); }
    void setC(double c)
    {
        if(c < -273.15) throw invalid_argument("Too low");
        celsius = c;
    }
    double getC() const { return celsius; }
};
```
---
# Задачи
---

1. Дефинирайте `struct Date { int d, m, y; }` и функция `bool isLeap(const Date&)`.
2. Създайте `struct Student { string name; int fn; double grade; }` и:
   * функция `void normalize(Student&)`, която ограничава `grade` в \[2.0, 6.0];
   * функция `void print(const Student&)`.
3. Имплементирайте клас `Vector` за динамичен масив от `double` с:
   * конструктор `Vector(size_t n)`, деструктор, оператор `[]` (const и non-const);
   * копиращ и премествaщ конструктор/присвояване (Rule of 5);
   * метод `size()`.
4. Напишете функция `join(const vector<string>&, const string& sep)`.
5. Реализирайте клас `Timer` с:
   * `start()`, `stop()`, `elapsed() const`; използвайте `std::chrono`;
   * демонстрирайте подаване на параметър по `const&` спрямо по стойност.
---
# Допълнително (по желание)
---
* `friend` функции/класове за оператори `<<`/`>>`.
* Оператори за сравнение `==`, `<=>` (C++20).
* Шаблони и препращащи референции (`T&&` + `std::forward`).
