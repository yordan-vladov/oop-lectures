# SOLID принципи

---

## Съдържание

- Какво представляват SOLID принципите
- Пример за всеки принцип
- Практическа задача

---

## Защо ни трябват SOLID принципите?

* По-четим код
* По-лесна поддръжка
* По-малко зависимости
* По-лесно тестване
* По-гъвкава архитектура

---

## Какво означава SOLID?

- **SOLID** е акроним от пет принципа:
    * Single Responsibility Principle
    * Open/Closed Principle
    * Liskov Substitution Principle
    * Interface Segregation Principle
    * Dependency Inversion Principle

---

## Single Responsibility Principle (SRP)

**Принцип:**
Един клас трябва да има само една причина за промяна.

---

## Лош пример

```cpp
class Report {
public:
    void generateReport();
    void saveToFile();
};
```

Класът има две отговорности – генериране и запис.

---

## Добър пример

```cpp
class Report {
public:
    void generateReport();
};

class ReportSaver {
public:
    void saveToFile(const Report& report);
};
```

Разделяме логиката и подобряваме поддръжката.

---

## Open/Closed Principle (OCP)

**Принцип:**
Софтуерните обекти трябва да са отворени за разширяване, но затворени за модификация.

---

## Лош пример

```cpp
class Discount {
public:
    double calculate(int type) {
        if (type == 1) return 0.1;
        if (type == 2) return 0.2;
        return 0;
    }
};
```

Добавянето на нов тип изисква промяна в класа.

---

## Добър пример

```cpp
class Discount {
public:
    virtual double calculate() const = 0;
    virtual ~Discount() = default;
};

class StudentDiscount : public Discount {
public:
    double calculate() const override { return 0.1; }
};

class VIPDiscount : public Discount {
public:
    double calculate() const override { return 0.2; }
};
```

Добавяме нов клас без да променяме съществуващия код.

---

## Liskov Substitution Principle (LSP)

**Принцип:**
Обект от производен клас трябва да може да замести обект от базов клас без да нарушава логиката.

---

## Лош пример

```cpp
class Bird {
public:
    virtual void fly() {}
};

class Penguin : public Bird {
public:
    void fly() override {
        throw std::logic_error("Penguins can't fly");
    }
};
```

Пингвинът нарушава очакваното поведение.

---

## Добър пример

```cpp
class Bird {};

class FlyingBird : public Bird {
public:
    virtual void fly() = 0;
};

class Sparrow : public FlyingBird {
public:
    void fly() override {}
};

class Penguin : public Bird {};
```

Йерархията е коректна и логиката не се нарушава.

---

## Interface Segregation Principle (ISP)

**Принцип:**
Клас не трябва да бъде принуждаван да имплементира методи, които не използва.

---

## Лош пример

```cpp
class Worker {
public:
    virtual void work() = 0;
    virtual void eat() = 0;
};

class Robot : public Worker {
public:
    void work() override {}
    void eat() override {}
};
```

Роботът няма нужда от метод eat().

---

## Добър пример

```cpp
class Workable {
public:
    virtual void work() = 0;
};

class Eatable {
public:
    virtual void eat() = 0;
};

class Human : public Workable, public Eatable {
public:
    void work() override {}
    void eat() override {}
};

class Robot : public Workable {
public:
    void work() override {}
};
```

Интерфейсите са разделени според нуждите.

---

## Dependency Inversion Principle (DIP)

**Принцип:**
Зависимостите трябва да са към абстракции, а не към конкретни класове.

---

## Лош пример

```cpp
class MySQLDatabase {
public:
    void connect();
};

class Application {
private:
    MySQLDatabase db;
};
```

Има твърда зависимост към конкретна база.

---

## Добър пример

```cpp
class Database {
public:
    virtual void connect() = 0;
    virtual ~Database() = default;
};

class MySQLDatabase : public Database {
public:
    void connect() override {}
};

class Application {
private:
    Database& db;
public:
    Application(Database& database) : db(database) {}
};
```

Можем лесно да сменим имплементацията и да тестваме системата.

---

## Пример

- Казус: Онлайн магазин
- Имаме класове:
    * Product
    * Discount
    * PaymentMethod
    * Database

---

- При правилно използване на SOLID:
    * Добавяме нови методи на плащане
    * Добавяме нови отстъпки
    * Сменяме база данни
    * Разширяваме функционалността без да нарушаваме съществуващия код

---

## Чести грешки

* Прекомерно наследяване
* God class
* Силно свързан код
* Липса на абстракции

---

## Обобщение

| Принцип | Основна идея                          |
| ------- | ------------------------------------- |
| SRP     | Един клас – една отговорност          |
| OCP     | Разширявай без да променяш            |
| LSP     | Производният клас не нарушава базовия |
| ISP     | Малки, специализирани интерфейси      |
| DIP     | Зависимост от абстракции              |

---

## Практическа задача

- Да се разработи система за плащане с:
    * Поне два вида PaymentMethod
    * Абстрактен базов клас
    * Спазване на поне три SOLID принципа
