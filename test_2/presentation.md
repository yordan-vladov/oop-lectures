# Подготовка за тест - Теми 15–25

---

## Какво включва тестът?

- **10 затворени въпроса** - избор на верен отговор (1 т. всеки)
- **5 отворени въпроса** - кратки обяснения и код (4 т. всеки)
- **Продължителност:** 45 минути
- **Теми:** Design Patterns, SOLID, Singleton, Factory, Abstract Factory, Builder, Adapter, Decorator, Observer, Strategy, Dependency Injection

---

# Design Patterns (Тема 15)

---

## Какво са Design Patterns?

- Повтарящи се решения на честисрещани проблеми в софтуерния дизайн
- Концептуални шаблони - не готов код
- Независими от конкретен програмен език

---

## Три категории (GoF)

| Категория  | Описание                              | Примери                          |
| ---------- | ------------------------------------- | -------------------------------- |
| Creational | Контрол при създаване на обекти       | Singleton, Factory, Builder      |
| Structural | Комбиниране на класове в структури    | Adapter, Decorator               |
| Behavioral | Взаимодействие и разпределение на отговорности | Observer, Strategy    |

> **Architectural Patterns** (MVC, Microservices) работят на ниво система, а не класове.

---

# SOLID принципи (Тема 16)

---

## S - Single Responsibility

**Един клас = една причина за промяна.**

```cpp
// Лошо: Report генерира И записва
// Добро: Report генерира, ReportSaver записва
```

---

## O - Open/Closed

**Отворен за разширение, затворен за модификация.**

```cpp
// Добавяй нови типове чрез нови класове,
// без да променяш съществуващ код.
class Discount { virtual double calculate() = 0; };
class VIPDiscount : public Discount { ... };
```

---

## L - Liskov Substitution

**Производен клас не трябва да нарушава поведението на базовия.**

```cpp
// Лошо: Penguin : Bird, но fly() хвърля exception
// Добро: Bird + FlyingBird (отделна йерархия)
```

---

## I - Interface Segregation

**Клас не трябва да имплементира методи, които не използва.**

```cpp
// Лошо: Robot имплементира eat()
// Добро: Workable + Eatable - отделни интерфейси
```

---

## D - Dependency Inversion

**Зависимостите да са към абстракции, не към конкретни класове.**

```cpp
// Лошо: Application държи MySQLDatabase
// Добро: Application държи Database& (абстракция)
```

---

# Singleton (Тема 17)

---

## Основна идея

- Само една инстанция на класа
- Private конструктор
- Забранено копиране и местене
- Static метод `getInstance()`

---

## Meyers Singleton (препоръчван)

```cpp
class Singleton {
private:
    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
public:
    static Singleton& getInstance() {
        static Singleton instance;  // lazy + thread-safe
        return instance;
    }
};
```

---

## Кога е подходящ?

- Logger, DatabaseConnection, Config
- Когато логически има само **една** инстанция

## Недостатъци

- Трудно тестване, скрита зависимост, нарушава SRP

---

# Factory (Тема 18)

---

## Проблем без Factory

```cpp
if (type == "circle") return new Circle();
else if (type == "square") return new Square();
// Всеки нов тип → промяна на съществуващ код
```

---

## Simple Factory

```cpp
class ShapeFactory {
public:
    static unique_ptr<Shape> createShape(string type) {
        if (type == "circle") return make_unique<Circle>();
        if (type == "square") return make_unique<Square>();
        return nullptr;
    }
};
```

---

## Registry-based Factory (спазва OCP)

- Фабриката не знае конкретните класове
- Всеки клас **сам се регистрира**
- Добавянето на нов тип не изисква промяна на фабриката

---

# Abstract Factory (Тема 19)

---

## Разлика спрямо Factory

| Factory                  | Abstract Factory                      |
| ------------------------ | ------------------------------------- |
| Създава **един** продукт | Създава **семейство** от продукти     |
| Една фабрика             | Множество фабрики                     |
| По-прост                 | Гарантира съвместимост между продукти |

---

## Пример: GUI

```cpp
class GUIFactory {
public:
    virtual unique_ptr<Button> createButton() = 0;
    virtual unique_ptr<CheckBox> createCheckBox() = 0;
};

class WindowsFactory : public GUIFactory { ... };
class MacFactory     : public GUIFactory { ... };
```

Клиентът не знае дали работи с Windows или Mac.

---

# Builder (Тема 20)

---

## Проблем

```cpp
Car car("V8", true, false, "Red", 4, ...); // нечетим, грешки лесно
```

---

## Структура

- **Product** - крайният обект
- **Builder** - интерфейс за изграждане
- **ConcreteBuilder** - конкретна реализация
- **Director** - управлява реда на стъпките

---

## Fluent Builder (модерен вариант)

```cpp
Computer pc = ComputerBuilder()
                .setCPU("Ryzen 7")
                .setRAM("16GB")
                .setStorage("512GB SSD")
                .build();
```

---

# Adapter (Тема 21)

---

## Роли

- **Target** - интерфейсът, който клиентът очаква
- **Adaptee** - съществуващият клас с несъвместим интерфейс
- **Adapter** - превежда единия в другия
- **Client** - работи само с Target

---

## Пример

```cpp
class PlayerAdapter : public MediaPlayer {  // Target
private:
    AdvancedPlayer advancedPlayer;          // Adaptee
public:
    void play(const string& file) override {
        advancedPlayer.startPlayback(file); // превод
    }
};
```

> Аналогия: зарядното за телефон е Adapter между 220V и 5V.

---

## Object Adapter vs Class Adapter

| Object Adapter     | Class Adapter      |
| ------------------ | ------------------ |
| Използва композиция | Използва наследяване |
| По-гъвкав          | По-ограничен        |
| По-често използван | По-рядко използван  |

---

# Decorator (Тема 22)

---

## Проблем: Class Explosion

```cpp
// Наследяване → CoffeeWithMilk, CoffeeWithSugar,
// CoffeeWithMilkAndSugar, ... (расте експоненциално)
```

---

## Идея

- Обвиване на обект с друг обект **от същия интерфейс**
- Добавяне на поведение **runtime**, без промяна на оригиналния клас

---

## Структура

```cpp
class CoffeeDecorator : public Coffee {
protected:
    unique_ptr<Coffee> coffee;
public:
    CoffeeDecorator(unique_ptr<Coffee> c) : coffee(move(c)) {}
};

class MilkDecorator : public CoffeeDecorator {
public:
    double cost() const override { return coffee->cost() + 1.5; }
};
```

Използване: `Sugar(Milk(SimpleCoffee()))` - верига от декоратори.

---

# Observer (Тема 23)

---

## Роли

| Роля            | Описание                                      |
| --------------- | --------------------------------------------- |
| Subject         | Поддържа списък наблюдатели, уведомява ги     |
| Observer        | Интерфейс с метод `update()`                  |
| ConcreteSubject | Реален обект със състояние                    |
| ConcreteObserver| Реален наблюдател, реагира на промените       |

---

## Пример

```cpp
sensor.attach(&display);
sensor.attach(&logger);
sensor.setValue(42);  // notify() → display.update(42), logger.update(42)
```

---

## Push vs Pull

| Push                       | Pull                           |
| -------------------------- | ------------------------------ |
| Subject изпраща данните    | Observer взима данните сам     |
| По-прост                   | По-гъвкав, повече контрол      |

---

## Кога се използва?

GUI събития, stock market, game engines, notification системи, MVC

---

# Strategy (Тема 24)

---

## Проблем

```cpp
if (method == "card") ...
else if (method == "paypal") ...
// Нов метод → промяна в съществуващ код → нарушава OCP
```

---

## Решение: Strategy Pattern

```cpp
class PaymentStrategy { virtual void pay(double) = 0; };
class CreditCardPayment : public PaymentStrategy { ... };
class PayPalPayment     : public PaymentStrategy { ... };

class PaymentContext {
    unique_ptr<PaymentStrategy> strategy;
public:
    void setStrategy(unique_ptr<PaymentStrategy> s) { strategy = move(s); }
    void executePayment(double amount) { strategy->pay(amount); }
};
```

---

## Ключови точки

- Алгоритмите се капсулират в отделни класове
- Поведението може да се сменя **runtime**
- Добавянето на нова стратегия = нов клас, без промяна на Context
- Спазва OCP

---

# Dependency Injection (Тема 25)

---

## Проблем: Tight Coupling

```cpp
class Car {
    Engine engine;  // Car сам създава зависимостта си
};
// Искаме ElectricEngine → трябва да променим Car
```

---

## Решение: DI

```cpp
class Car {
    IEngine* engine;
public:
    Car(IEngine* eng) : engine(eng) {}  // получава зависимостта отвън
};
```

---

## Видове DI

| Вид                  | Описание                                  |
| -------------------- | ----------------------------------------- |
| Constructor Injection| Подава се в конструктора **(най-честото)** |
| Setter Injection     | Подава се чрез setter метод               |
| Interface Injection  | По-рядко използван                        |

---

## Предимства

- Ниска свързаност (loose coupling)
- Лесно тестване (подаваме mock обект)
- Спазва DIP от SOLID
- По-добра разширяемост

---

# Обобщителна таблица

---

| Шаблон           | Категория   | Основна цел                                      |
| ---------------- | ----------- | ------------------------------------------------ |
| Singleton        | Creational  | Само една инстанция                              |
| Factory          | Creational  | Централизирано създаване на един продукт         |
| Abstract Factory | Creational  | Създаване на семейство от свързани продукти      |
| Builder          | Creational  | Поетапно изграждане на сложен обект              |
| Adapter          | Structural  | Свързване на несъвместими интерфейси             |
| Decorator        | Structural  | Динамично добавяне на поведение                  |
| Observer         | Behavioral  | Уведомяване на множество обекти при промяна      |
| Strategy         | Behavioral  | Взаимозаменяеми алгоритми                        |

---

# Честа грешка - нарушения на OCP

---

Разпознайте кога се нарушава Open/Closed:

- **if/else** за различни типове → Factory или Strategy
- **Наследяване** за всяка комбинация → Decorator
- **Директна зависимост** от конкретен клас → DI + абстракция

---

# Успех на теста!
