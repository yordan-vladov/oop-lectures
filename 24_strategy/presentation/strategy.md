# Strategy

---

## Съдържание

* Какво е Strategy pattern
* Какви проблеми решава този шаблон
* Какво е принципът Open/Closed
* Как се реализира Strategy в C++

---

## Припомняне: Условна логика в C++

Често имаме код като:

```cpp
if(type == "car")
    calculateCarPrice();
else if(type == "truck")
    calculateTruckPrice();
else if(type == "bike")
    calculateBikePrice();
```

---

* **Проблем:**

  * Много if/else конструкции
  * Трудно разширение
  * Нарушение на Open/Closed принципа
  * Смесване на логика и алгоритми

---

## Какъв проблем решава Strategy?

Имаме клас, който трябва да използва различни алгоритми:

* Различни начини за сортиране
* Различни стратегии за плащане
* Различни алгоритми за компресия
* Различни поведения на персонажи в игра

---

Вместо:

```cpp
if(strategy == "fast") ...
else if(strategy == "secure") ...
```

използваме обекти със сменяемо поведение.

---

## Принцип Open/Closed (OCP)

### Дефиниция

Софтуерните компоненти трябва да бъдат:

* Отворени за разширение
* Затворени за модификация

---

Това означава:

* Добавяме нов алгоритъм чрез нов клас
* Не променяме съществуващ код

---

### Пример за нарушение на OCP

```cpp
double calculateDiscount(string customerType) {
    if(customerType == "regular") ...
    else if(customerType == "vip") ...
    else if(customerType == "employee") ...
}
```

Всеки нов тип изисква промяна в кода.

---

## Как Strategy помага?

* Алгоритмите се капсулират в отделни класове
* Клиентът работи чрез интерфейс
* Поведението може да се сменя по време на изпълнение
* Лесно добавяне на нови стратегии

---

## Какво е Strategy Pattern?

Strategy е шаблон за проектиране, който дефинира семейство от алгоритми, капсулира всеки от тях в отделен клас и ги прави взаимозаменяеми.

Контекстният клас използва стратегия чрез интерфейс, без да знае конкретната реализация.

---

## UML идея

* Strategy – интерфейс
* ConcreteStrategyA, ConcreteStrategyB – реализации
* Context – използва стратегията

---

![strategy_uml](https://www.tutorialspoint.com/design_pattern/images/strategy_pattern_uml_diagram.jpg)

---

## Пример: Payment Strategy

Ще създадем система за плащане с различни методи:

* CreditCard
* PayPal
* Crypto

---

## Абстрактен клас Strategy

```cpp
class PaymentStrategy {
public:
    virtual void pay(double amount) = 0;
    virtual ~PaymentStrategy() {}
};
```

---

## Конкретни стратегии

```cpp
class CreditCardPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid " << amount << " using Credit Card\n";
    }
};

class PayPalPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid " << amount << " using PayPal\n";
    }
};

class CryptoPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid " << amount << " using Crypto\n";
    }
};
```

---

## Context клас

```cpp
#include <memory>

class PaymentContext {
private:
    std::unique_ptr<PaymentStrategy> strategy;

public:
    void setStrategy(std::unique_ptr<PaymentStrategy> newStrategy) {
        strategy = std::move(newStrategy);
    }

    void executePayment(double amount) {
        if(strategy)
            strategy->pay(amount);
    }
};
```

---

## Използване

```cpp
int main() {
    PaymentContext context;

    context.setStrategy(std::make_unique<CreditCardPayment>());
    context.executePayment(100);

    context.setStrategy(std::make_unique<PayPalPayment>());
    context.executePayment(200);

    context.setStrategy(std::make_unique<CryptoPayment>());
    context.executePayment(300);
}
```

---

## Подобрена версия: Dependency Injection

Можем да подадем стратегията през конструктора:

```cpp
class PaymentContext {
private:
    std::unique_ptr<PaymentStrategy> strategy;

public:
    PaymentContext(std::unique_ptr<PaymentStrategy> s)
        : strategy(std::move(s)) {}

    void executePayment(double amount) {
        strategy->pay(amount);
    }
};
```

---

## Проблем без Strategy

```cpp
class PaymentProcessor {
public:
    void pay(string method, double amount) {
        if(method == "card") ...
        else if(method == "paypal") ...
        else if(method == "crypto") ...
    }
};
```

---

Недостатъци:

* Огромни if/else блокове
* Трудно добавяне на нов метод
* Нарушение на OCP
* Трудно тестване

---

## Добавяне на нова стратегия

```cpp
class BankTransferPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid " << amount << " via Bank Transfer\n";
    }
};
```

Не променяме съществуващ код — само добавяме нов клас.

---

## Защо това спазва OCP?

* Контекстът не се променя
* Добавяме ново поведение чрез нов клас
* Алгоритмите са отделени от логиката

---

## Сравнение

| Без Strategy         | Strategy Pattern           |
| -------------------- | -------------------------- |
| if/else логика       | Полиморфизъм               |
| Трудно разширение    | Лесно добавяне на нов клас |
| Смесени отговорности | Разделени отговорности     |
| Нарушава OCP         | Спазва OCP                 |

---

## Кога да използваме Strategy?

* Когато имаме много варианти на алгоритъм
* Когато поведението трябва да се сменя динамично
* Когато искаме да избегнем if/else
* Когато искаме по-лесно тестване

---

## Предимства

* Спазване на Open/Closed
* По-чист код
* Лесно разширение
* Поведението може да се сменя runtime
* По-добра поддръжка

---

## Недостатъци

* Повече класове
* Допълнителна абстракция
* Клиентът трябва да знае стратегиите

---

## Практическа задача

Създайте система за сортиране със Strategy pattern.

---

### Изисквания:

* Абстрактен клас `SortStrategy` с метод `sort(vector<int>& data)`
* Конкретни стратегии:

  * BubbleSort
  * QuickSort
  * MergeSort
* Клас `SorterContext`
* Демонстрация в main()
* Да се използва `unique_ptr`

---

### Допълнително предизвикателство

Добавете нов алгоритъм `HeapSort`, без да променяте съществуващия код на контекста.

---

## Дискусия

* Кога Strategy е по-подходящ от Factory?
* Как Strategy подпомага OCP?
* Какви са реални примери от индустрията?
