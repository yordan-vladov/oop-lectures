# Decorator

---

## Съдържание

* Какво е Decorator pattern
* Какви проблеми решава този шаблон
* Принципът Open/Closed и Decorator
* Как се реализира Decorator в C++
* Реален пример
* Предимства и недостатъци

---

## Припомняне: Разширяване на поведение чрез наследяване

Обикновено добавяме функционалност чрез наследяване:

```cpp
class Coffee {
public:
    virtual double cost() { return 5.0; }
};
```

```cpp
class MilkCoffee : public Coffee {
public:
    double cost() override { return 6.0; }
};
```

---

* **Проблем:**

  * Много комбинации → много класове
  * Explosion of classes (class explosion)
  * Трудно разширяване
  * Нарушаване на гъвкавостта

---

## Какъв проблем решава Decorator?

Имаме система за напитки:

* Coffee
* Добавки:

  * Milk
  * Sugar
  * Chocolate

---

Ако използваме наследяване:

* CoffeeWithMilk
* CoffeeWithSugar
* CoffeeWithMilkAndSugar
* CoffeeWithMilkSugarChocolate

Броят на класовете расте експоненциално.

---

## Идея на Decorator

Вместо да наследяваме:

- Обвиваме обекта с друг обект.

Decorator:

* Добавя ново поведение
* Без да променя оригиналния клас
* По време на изпълнение (runtime)

---

## Принцип Open/Closed (OCP)

Decorator спазва OCP:

* Отворени за разширение → добавяме нов Decorator
* Затворени за модификация → не променяме стар код

---

## Какво е Decorator Pattern?

Decorator е шаблон за проектиране, който позволява динамично добавяне на нова функционалност към обекти чрез обвиване в друг обект със същия интерфейс.

---

## Структура на Decorator

Основни роли:

* Component – общ интерфейс
* ConcreteComponent – реален обект
* Decorator – базов декоратор
* ConcreteDecorator – конкретно разширение

---

## Пример: Coffee система

### Абстрактен компонент

```cpp
class Coffee {
public:
    virtual double cost() const = 0;
    virtual std::string description() const = 0;
    virtual ~Coffee() = default;
};
```

---

### Concrete Component

```cpp
class SimpleCoffee : public Coffee {
public:
    double cost() const override {
        return 5.0;
    }

    std::string description() const override {
        return "Simple Coffee";
    }
};
```

---

## Базов Decorator

```cpp
#include <memory>

class CoffeeDecorator : public Coffee {
protected:
    std::unique_ptr<Coffee> coffee;

public:
    CoffeeDecorator(std::unique_ptr<Coffee> c)
        : coffee(std::move(c)) {}
};
```

---

## Конкретни Decorators

### Milk Decorator

```cpp
class MilkDecorator : public CoffeeDecorator {
public:
    MilkDecorator(std::unique_ptr<Coffee> c)
        : CoffeeDecorator(std::move(c)) {}

    double cost() const override {
        return coffee->cost() + 1.5;
    }

    std::string description() const override {
        return coffee->description() + ", Milk";
    }
};
```

---

### Sugar Decorator

```cpp
class SugarDecorator : public CoffeeDecorator {
public:
    SugarDecorator(std::unique_ptr<Coffee> c)
        : CoffeeDecorator(std::move(c)) {}

    double cost() const override {
        return coffee->cost() + 0.5;
    }

    std::string description() const override {
        return coffee->description() + ", Sugar";
    }
};
```

---

## Използване

```cpp
int main() {

    std::unique_ptr<Coffee> coffee =
        std::make_unique<SimpleCoffee>();

    coffee = std::make_unique<MilkDecorator>(std::move(coffee));
    coffee = std::make_unique<SugarDecorator>(std::move(coffee));

    std::cout << coffee->description() << std::endl;
    std::cout << coffee->cost() << std::endl;
}
```

---

## Какво се случва?

Структурата става:

```
SugarDecorator
      ↓
MilkDecorator
      ↓
SimpleCoffee
```

Всеки декоратор добавя поведение.

---

## Добавяне на нов Decorator (Chocolate)

```cpp
class ChocolateDecorator : public CoffeeDecorator {
public:
    ChocolateDecorator(std::unique_ptr<Coffee> c)
        : CoffeeDecorator(std::move(c)) {}

    double cost() const override {
        return coffee->cost() + 2.0;
    }

    std::string description() const override {
        return coffee->description() + ", Chocolate";
    }
};
```

---

* Не променяме стар код
* Добавяме само нов клас
* Спазваме OCP

---

## Реален пример: GUI компоненти

Decorator се използва често при:

* Scroll bars
* Borders
* Shadows
* Logging
* Compression
* Encryption

---

## Сравнение: Наследяване vs Decorator

| Наследяване         | Decorator           |
| ------------------- | ------------------- |
| Статично поведение  | Динамично поведение |
| Много класове       | Малко класове       |
| Компилационно време | Runtime             |
| По-малко гъвкаво    | Много гъвкаво       |

---

## Предимства

* Спазва Open/Closed принципа
* Гъвкаво комбиниране на поведение
* Избягва class explosion
* Поведение може да се добавя динамично

---

## Недостатъци

* Повече малки класове
* По-сложна структура
* Дебъгването може да е по-трудно
* Много обвивки могат да затруднят разбирането

---

## Кога да използваме Decorator?

* Когато искаме да добавяме функционалност динамично
* Когато наследяването води до твърде много класове
* Когато искаме гъвкави комбинации от поведение
* При UI библиотеки, middleware, logging системи

---

## Обобщение

* Decorator добавя поведение чрез обвиване на обекти
* Не променя оригиналния клас
* Спазва Open/Closed принципа
* Позволява динамично комбиниране на функционалност

---

## Практическа задача

Създайте Decorator система за текстов редактор.

---

### Изисквания

* Абстрактен клас `Text` с метод `render()`
* Клас `PlainText`
* Decorators:

  * BoldDecorator
  * ItalicDecorator
  * UnderlineDecorator
* Да се използва `unique_ptr`
* Демонстрация в main()

---

### Допълнително предизвикателство

Добавете нов decorator `ShadowDecorator`, без да променяте клиентския код.

---

## Дискусия

* Как Decorator подпомага спазването на OCP?
* Каква е разликата между Decorator и наследяване?
* Кога Decorator НЕ е подходящ?
* Какви реални системи използват Decorator?
