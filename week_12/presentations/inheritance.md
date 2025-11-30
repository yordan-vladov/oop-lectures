# Наследяване в C++

---

## Какво е наследяване?

* Механизъм на ООП, който позволява:
  * създаване на **нови класове**, базирани на съществуващи;
  * **повторно използване** на код;
  * изграждане на **йерархии** и по-ясна структура на програмата.
---
* Основна идея:
  * Общият код → в **базов клас**
  * Специфично поведение → в **производни класове**
---
## Базов и производен клас

```cpp
class Base {
public:
    void greet() const {
        std::cout << "Hello from Base!\n";
    }
};

class Derived : public Base {
public:
    void greetDerived() const {
        std::cout << "Hello from Derived!\n";
    }
};
```

---
* `Derived` **наследява** всички публични/защитени членове на `Base`.
* Производният клас може да:

  * използва наследените методи;
  * добавя нови.

---

## Видове наследяване (`public`, `protected`, `private`)

---
### Най-често използвано: **public наследяване**

```
public Base → public в Derived
protected Base → protected в Derived
private Base → inaccessible
```

---

### `protected` наследяване

```
public Base → protected в Derived
protected Base → protected в Derived
private Base → inaccessible
```

---

### `private` наследяване

```
public Base → private в Derived
protected Base → private в Derived
private Base → inaccessible
```

**Извод:**

> В 99% от случаите използваме **public наследяване**.

---

## Конструктори и деструктори

* Конструкторът на базовия клас се вика **преди** конструктора на производния.
* Деструкторът – **в обратен ред**.
---
```cpp
class Base {
public:
    Base()  { std::cout << "Base()\n"; }
    ~Base() { std::cout << "~Base()\n"; }
};

class Derived : public Base {
public:
    Derived()  { std::cout << "Derived()\n"; }
    ~Derived() { std::cout << "~Derived()\n"; }
};
```
---
## Предаване на параметри към базов конструктор

```cpp
class Animal {
public:
    Animal(const std::string& n) : name(n) {}
protected:
    std::string name;
};

class Dog : public Animal {
public:
    Dog(const std::string& n, int age)
        : Animal(n), age(age) {}
private:
    int age;
};
```

* Производният клас **трябва** да извика конструктора на базовия.
---
## Полиморфизъм и виртуални функции

* Чрез `virtual` базовият клас позволява **презаписване** на методи.

```cpp
class Animal {
public:
    virtual void speak() const {
        std::cout << "Some sound\n";
    }
};
```

---

### Презаписване (`override`)

```cpp
class Dog : public Animal {
public:
    void speak() const override {
        std::cout << "Bark!\n";
    }
};
```

* Без `virtual` → работи статично свързване (по тип на променливата)
* С `virtual` → динамично свързване (по реалния тип на обекта)

---

## Виртуален деструктор

* Задължителен, ако ще трием през указател към базов клас.

```cpp
class Base {
public:
    virtual ~Base() = default;
};
```

---

## Абстрактни класове и чисто виртуални функции

* Ако клас съдържа **чисто виртуална функция**, той е **абстрактен**.

```cpp
class Shape {
public:
    virtual double area() const = 0; // чисто виртуална
};
```
---
* Не може да се създава обект от абстрактен клас.
* Производните **трябва да имплементират** всички чисто виртуални функции.

---
## Примерна йерархия: `Shape`
![inheritance_1](/Attachments/inheritance_1.png)

---
### Пример

```cpp
class Shape {
public:
    virtual double area() const = 0;
};

class Rectangle : public Shape {
public:
    Rectangle(double w, double h) : w(w), h(h) {}
    double area() const override { return w * h; }
private:
    double w, h;
};

class Circle : public Shape {
public:
    Circle(double r) : r(r) {}
    double area() const override { return 3.14 * r * r; }
private:
    double r;
};
```

---

## Множествено наследяване

```cpp
class A { };
class B { };
class C : public A, public B { };
```

* Позволено е в C++
---
* Но често създава проблеми:
  * конфликти на имена;
  * диамантено наследяване;
  * нужда от виртуално наследяване.
* Използвайте **рядко** – само когато е наистина необходимо.
---

## Проблем: диамантено наследяване

![inheritance_2](/Attachments/inheritance_2.png)

---
### Решение: виртуално наследяване

```cpp
class A { };
class B : virtual public A { };
class C : virtual public A { };
class D : public B, public C { };
```

---
## Наследяване vs Композиция

| Наследяване              | Композиция         |
| ------------------------ | ------------------ |
| „Е *е вид* B“            | „A *има* B“        |
| Създава силна връзка     | По-гъвкаво         |
| Лесно води до усложнения | Предпочитан подход |

**Златно правило:**

> Използвайте наследяване само когато това е *концептуално логично*.

---

## Добри практики при наследяване

* Използвайте **virtual** там, където трябва да има полиморфизъм.
* Използвайте `override`, за да избегнете грешки.
* Добавяйте **виртуален деструктор** в базови класове.
* Не злоупотребявайте с множествено наследяване.
* Ясно дефинирайте отговорностите – всеки клас трябва да има една роля (SRP).

---
## Обобщение: наследяването в C++

* Позволява изграждане на класови йерархии.
* Поддържа **полиморфизъм** чрез виртуални функции.
* Абстрактните класове служат като интерфейси.
* Внимание при множествено и виртуално наследяване!
* Композицията често е по-добрият инструмент.

---

## 4. **Задача: Йерархия от фигури**

Създайте проект със следната структура:

```text
project/
├─ include/
│  ├─ Shape.h
│  ├─ Rectangle.h
│  ├─ Circle.h
├─ src/
│  ├─ Rectangle.cpp
│  ├─ Circle.cpp
│  ├─ main.cpp
├─ CMakeLists.txt
```
---
### Изисквания:

* `Shape` – абстрактен клас с:
  * `virtual double area() const = 0;`
  * виртуален деструктор.
* `Rectangle` и `Circle` – наследници, които имплементират `area()`.
---
* В `main.cpp`:
  * създайте масив от `Shape*`
  * покажете полиморфно поведение (извикване на `area()`).
* Използвайте **CMake**, за да:
  * компилирате отделните `.cpp` файлове;
  * линкнете `Rectangle` и `Circle` към изпълнимия файл.