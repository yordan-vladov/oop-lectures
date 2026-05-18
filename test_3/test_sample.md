# Контролно по ООП — Тема 3

**Продължителност:** 45 минути  
**Теми:** MVC, Функционално програмиране, Event-Driven Design, Message Broker, Микроуслуги  
**Език:** C++

---

## Въпрос 1 — MVC (10 точки)

Разгледайте следния непълен код на MVC приложение за управление на книги:

```cpp
class BookModel {
    std::vector<std::string> books;
    std::vector<std::function<void()>> observers;

public:
    void addObserver(std::function<void()> obs) {
        observers.push_back(obs);
    }

    void addBook(const std::string& title) {
        books.push_back(title);
        // (A)
    }

    const std::vector<std::string>& getBooks() const { return books; }
};

class BookView {
public:
    void display(const std::vector<std::string>& books) {
        std::cout << "=== Книги ===\n";
        for (const auto& b : books) std::cout << "- " << b << "\n";
    }
    std::string getTitle() {
        std::string t; std::cin >> t; return t;
    }
};

class BookController {
    BookModel& model;
    BookView&  view;
public:
    BookController(BookModel& m, BookView& v) : model(m), view(v) {
        // (B)
    }

    void addBook() {
        std::string title = view.getTitle();
        // (C)
    }
};
```

**а)** Какво трябва да се напише на мястото на коментар **(A)**, за да работи правилно Observer механизмът? _(2 т.)_

**б)** Попълнете тялото на конструктора на `BookController` — **(B)** — така че View автоматично да се обновява при промяна на Model. _(4 т.)_

**в)** Попълнете тялото на `addBook()` — **(C)**. _(2 т.)_

**г)** В кой компонент (Model / View / Controller) трябва да се добави логика за валидация (например: заглавието не може да е по-кратко от 3 символа)? Обосновете накратко. _(2 т.)_

---

## Въпрос 2 — Функционално програмиране (10 точки)

Дадена е следната структура и вектор:

```cpp
struct Product {
    std::string name;
    double price;
    int stock;
};

std::vector<Product> products = {
    {"Лаптоп",     1500.0, 5},
    {"Мишка",        25.0, 0},
    {"Монитор",     650.0, 3},
    {"Клавиатура",   80.0, 0},
    {"Слушалки",    120.0, 8},
};
```

Използвайки само `std::copy_if`, `std::transform` и `std::accumulate` (без ръчни `for` цикли), напишете код, който:

**а)** Филтрира само продуктите с `stock > 0`. _(3 т.)_

**б)** Извлича цената (`price`) на всеки наличен продукт в нов вектор `double`. _(3 т.)_

**в)** Изчислява **средната цена** на наличните продукти и я отпечатва. _(4 т.)_

> **Подсказка:** `std::accumulate` е в `<numeric>`, а `std::copy_if` и `std::transform` — в `<algorithm>`.

---

## Въпрос 3 — Event-Driven Design (10 точки)

**а)** Обяснете каква е ключовата разлика между **Observer pattern** и **Event-Driven Design с EventBus** по отношение на зависимостите между компонентите. _(3 т.)_

**б)** Разгледайте кода по-долу и отговорете на въпросите:

```cpp
class Hero {
    EventBus& bus;
    int hp = 100;
public:
    Hero(EventBus& bus) : bus(bus) {}

    void takeDamage(int amount) {
        hp -= amount;
        bus.publish(Event("heroDamaged", hp));
        if (hp <= 0)
            bus.publish(Event("heroDied"));
    }
};

class SoundSystem {
public:
    SoundSystem(EventBus& bus) {
        bus.subscribe("heroDamaged", [](const Event& e) {
            std::cout << "[Sound] hurt.wav\n";
        });
        bus.subscribe("heroDied", [](const Event& e) {
            std::cout << "[Sound] death.wav\n";
        });
    }
};
```

- Колко зависимости има `Hero` от конкретни системи (SoundSystem, HealthBar и т.н.)? _(1 т.)_
- Ако искаме да добавим нова система `AchievementSystem`, която реагира на `"heroDied"`, трябва ли да се промени класът `Hero`? Защо? _(2 т.)_

**в)** Напишете класа `AchievementSystem`, който при събитие `"heroDied"` отпечатва `[Achievements] Unlocked: First Death`. _(4 т.)_

---

## Въпрос 4 — Message Broker (10 точки)

**а)** Обяснете разликата между **Queue** (Point-to-Point) и **Topic** (Publish-Subscribe) в Message Broker. Кога е подходящо да използвате всеки от двата режима? Дайте по един реален пример. _(4 т.)_

**б)** Разгледайте следния код и посочете **два проблема**, които Message Broker решава в сравнение с директната комуникация: _(2 т.)_

```cpp
// БЕЗ Message Broker
class PaymentService {
    ShippingService*  shipping;
    EmailService*     email;
    AnalyticsService* analytics;

public:
    void processPayment(const Order& order) {
        shipping->schedule(order);
        email->sendReceipt(order);
        analytics->record(order);
    }
};
```

**в)** Пренапишете `processPayment()` така, че да използва `MessageBroker` с topic `"payments"`, вместо директни извиквания. Приемете, че имате достъп до обект `MessageBroker& broker`. _(4 т.)_

---

## Въпрос 5 — Микроуслуги (10 точки)

**а)** Какво е **API Gateway** и каква е неговата роля в архитектура с микроуслуги? _(2 т.)_

**б)** Какво е **Circuit Breaker** и защо е важен при синхронна комуникация между услуги? _(3 т.)_

**в)** Попълнете следния код на `ApiGateway`, така че заявки към `/users/*` да се препращат към `"UserService"`, а към `/orders` — към `"OrderService"`. Ако услугата не е достъпна в `registry`, върнете отговор с код `503`. _(5 т.)_

```cpp
class ApiGateway {
    ServiceRegistry& registry;
public:
    explicit ApiGateway(ServiceRegistry& reg) : registry(reg) {}

    Response route(const Request& req) {
        // Попълнете тук
    }

private:
    Response forward(const std::string& serviceName, const Request& req) {
        // Попълнете тук
    }
};
```
