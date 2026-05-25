# Подготовка за Контролно 3 — ООП

**Теми:** MVC, Функционално програмиране, Event-Driven Design, Message Broker, Микроуслуги  
**Структура:** 5 въпроса × 10 точки = 50 точки | 45 минути

---

## Честота на въпросите по теми (от всички варианти)

| Тема | Типове въпроси |
|------|----------------|
| MVC | Попълване на код (Observer), рефакторинг на "spaghetti code", теория за компонентите |
| Функционално | `copy_if` + `transform` + `accumulate` вериги, функтори, ламбди, `std::bind`, `compose` |
| Event-Driven | Написване на EventEmitter/EventBus, Subscribe/Publish, разлика с Observer |
| Message Broker | Queue vs Topic, Producer/Consumer код, разлика с EventBus |
| Микроуслуги | API Gateway routing, Circuit Breaker, Service Registry, комуникация |

---

## Тема 1: MVC

### Трите компонента

| Компонент | Отговорност | НЕ трябва да |
|-----------|-------------|--------------|
| **Model** | Данните + бизнес логика + Observer механизъм | Знае за View, прави I/O |
| **View** | Показване на данни + четене на вход | Съдържа бизнес логика |
| **Controller** | Свързва Model и View, обработва входа | Съдържа UI логика или данни |

> **Валидацията** (напр. дължина на заглавие) е в **Model** — там е бизнес логиката.

### Шаблон — пълна MVC реализация

```cpp
// MODEL — пази данните и уведомява наблюдателите
class SomeModel {
    std::vector<std::string> items;
    std::vector<std::function<void()>> observers;

public:
    void addObserver(std::function<void()> obs) {
        observers.push_back(obs);
    }

    void notifyObservers() {
        for (auto& o : observers) o();
    }

    void addItem(const std::string& item) {
        items.push_back(item);
        notifyObservers();   // <-- (A) това се търси!
    }

    void removeItem(int index) {
        if (index >= 0 && index < (int)items.size()) {
            items.erase(items.begin() + index);
            notifyObservers();  // <-- (B) при всяка промяна
        }
    }

    const std::vector<std::string>& getItems() const { return items; }
};

// VIEW — само показва и чете вход
class SomeView {
public:
    void display(const std::vector<std::string>& items) {
        for (const auto& i : items) std::cout << "- " << i << "\n";
    }
    std::string getInput() {
        std::string s; std::cin >> s; return s;
    }
};

// CONTROLLER — свързва двете чрез Observer
class SomeController {
    SomeModel& model;
    SomeView&  view;
public:
    SomeController(SomeModel& m, SomeView& v) : model(m), view(v) {
        // (C) — свързваме Model с View чрез lambda
        model.addObserver([this]() {
            this->view.display(this->model.getItems());
        });
    }

    void addItem() {
        std::string input = view.getInput();
        model.addItem(input);  // (D)
    }
};
```

### MVVM — ключова разлика

- **MVC**: Controller дърпа данните, обновява View ръчно
- **MVVM**: ViewModel — View се "свързва" директно (**Data Binding**), при промяна в ViewModel → View се обновява **автоматично**

```cpp
// ViewModel задейства callback при промяна
void setTemperature(double c) {
    celsius = c;
    if (onChanged) onChanged(celsius, celsius * 9.0 / 5.0 + 32); // auto update
}
```

### Типични нарушения (Вариант 3)

```cpp
// ГРЕШНО:
class ScoreModel {
    void addPoints(int pts) {
        score += pts;
        std::cout << "Резултат: " << score; // Model прави I/O!
    }
};

class ScoreController {
    void displayHighScore() {
        std::cout << "*** РЕКОРД ***"; // Controller съдържа UI логика!
    }
};
```

**Поправка**: Преместете `cout` в **View**. Controller само извиква `view.displayHighScore(model.getScore())`.

### Множество View-та

```cpp
// Model-ът просто уведомява всички observer-и
model.addObserver([&]() { consoleView.display(model.getData()); });
model.addObserver([&]() { htmlView.render(model.getData()); });
```

---

## Тема 2: Функционално програмиране

### Трите алгоритъма — шаблон за верига

```cpp
// 1. FILTER — std::copy_if
std::vector<T> filtered;
std::copy_if(source.begin(), source.end(),
             std::back_inserter(filtered),
             [](const T& x) { return /* условие */; });

// 2. MAP — std::transform
std::vector<U> mapped(filtered.size());
std::transform(filtered.begin(), filtered.end(),
               mapped.begin(),
               [](const T& x) { return x.someField; });

// 3. REDUCE — std::accumulate (#include <numeric>)
double total = std::accumulate(mapped.begin(), mapped.end(), 0.0);
double avg   = total / mapped.size();
```

### Пример от примерното контролно (Products)

```cpp
std::vector<Product> products = { {"Лаптоп", 1500.0, 5}, {"Мишка", 25.0, 0}, ... };

// а) Само с stock > 0
std::vector<Product> inStock;
std::copy_if(products.begin(), products.end(), std::back_inserter(inStock),
    [](const Product& p) { return p.stock > 0; });

// б) Извличане на цените
std::vector<double> prices(inStock.size());
std::transform(inStock.begin(), inStock.end(), prices.begin(),
    [](const Product& p) { return p.price; });

// в) Средна цена
double total = std::accumulate(prices.begin(), prices.end(), 0.0);
std::cout << "Средна цена: " << total / prices.size() << "\n";
```

### Функтори vs Ламбди

```cpp
// Функтор — когато логиката се използва многократно или е по-сложна
struct GradeFilter {
    int minGrade;
    explicit GradeFilter(int min) : minGrade(min) {}
    bool operator()(const Student& s) const { return s.grade >= minGrade; }
};

// Употреба
std::copy_if(students.begin(), students.end(), std::back_inserter(result),
    GradeFilter(60));

// Ламбда — когато логиката е еднократна и проста
auto minG = 60;
std::copy_if(students.begin(), students.end(), std::back_inserter(result),
    [minG](const Student& s) { return s.grade >= minG; });
```

> **Кога функтор?** Когато трябва наследяване, многократна употреба, или вътрешно по-сложно състояние.

### Частично прилагане — `std::bind` и ламбди

```cpp
double discount(double percent, double price) {
    return price * (1.0 - percent / 100.0);
}

// std::bind
auto discount10 = std::bind(discount, 10.0, std::placeholders::_1);
auto discount25 = std::bind(discount, 25.0, std::placeholders::_1);

// Ламбда (по-четимо)
auto discount10 = [](double price) { return price * 0.90; };
auto discount25 = [](double price) { return price * 0.75; };

// Употреба с transform
std::vector<double> prices = {100.0, 200.0, 300.0};
std::vector<double> discounted(prices.size());
std::transform(prices.begin(), prices.end(), discounted.begin(), discount10);
```

### Композиция на функции (Вариант 5)

```cpp
// compose(f, g)(x) = f(g(x))
auto compose = [](auto f, auto g) {
    return [f, g](auto x) { return f(g(x)); };
};

auto addOne    = [](int x) { return x + 1; };
auto doubled   = [](int x) { return x * 2; };
auto squared   = [](int x) { return x * x; };

// squared → doubled → addOne върху 3:
// addOne(doubled(squared(3))) = addOne(doubled(9)) = addOne(18) = 19
auto pipeline = compose(addOne, compose(doubled, squared));
std::cout << pipeline(3); // → 19
```

### Capture правила

| Capture | Смисъл |
|---------|--------|
| `[=]` | Всички локални по стойност (копие) — безопасно за callbacks |
| `[&]` | Всички локални по референция — внимавай за dangling ref! |
| `[x]` | Само `x` по стойност |
| `[&x]`| Само `x` по референция |

---

## Тема 3: Event-Driven Design

### Ключова разлика: Observer vs EventBus

| | Observer Pattern | Event-Driven (EventBus) |
|--|--|--|
| Зависимост | Subject знае за Observer интерфейс | Publisher НЕ знае за Subscriber |
| Посредник | Subject държи списък | EventBus е отделен компонент |
| Разширяване | Нов observer → промяна в Subject | Нов subscriber без промяна на Publisher |
| Debugging | По-лесно | По-трудно |

### Шаблон — EventEmitter

```cpp
struct Event {
    std::string type;
    std::any data;
    Event(const std::string& t) : type(t) {}
    Event(const std::string& t, std::any d) : type(t), data(d) {}
};

using Handler = std::function<void(const Event&)>;

class EventEmitter {
    std::unordered_map<std::string, std::vector<Handler>> handlers;
public:
    void on(const std::string& eventType, Handler handler) {
        handlers[eventType].push_back(handler);
    }
    void emit(const Event& event) {
        auto it = handlers.find(event.type);
        if (it != handlers.end())
            for (auto& h : it->second) h(event);
    }
};
```

### Шаблон — EventBus (Singleton)

```cpp
class EventBus {
    using Handler = std::function<void(const Event&)>;
    std::unordered_map<std::string, std::vector<Handler>> handlers;
public:
    static EventBus& getInstance() {
        static EventBus instance;
        return instance;
    }
    void subscribe(const std::string& eventType, Handler handler) {
        handlers[eventType].push_back(handler);  // <-- (A)
    }
    void publish(const Event& event) {
        auto it = handlers.find(event.type);
        if (it != handlers.end())
            for (auto& h : it->second) h(event); // <-- (B)
    }
private:
    EventBus() = default;
};
```

### Шаблон — Subscriber клас

```cpp
class AchievementSystem {
public:
    AchievementSystem(EventBus& bus) {
        bus.subscribe("heroDied", [](const Event& e) {
            std::cout << "[Achievements] Unlocked: First Death\n";
        });
    }
};

// С данни от събитието
class HealthBar {
public:
    HealthBar(EventBus& bus) {
        bus.subscribe("heroDamaged", [](const Event& e) {
            int hp = std::any_cast<int>(e.data);
            std::cout << "[HealthBar] HP: " << hp << "\n";
        });
    }
};

// С std::any_cast за string
class AuditLog {
public:
    AuditLog() {
        EventBus::getInstance().subscribe("userLoggedIn", [](const Event& e) {
            auto user = std::any_cast<std::string>(e.data);
            std::cout << "[Audit] Вход: " << user << "\n";
        });
    }
};
```

### Защо добавянето на нов subscriber НЕ изисква промяна на Publisher

Button публикува `"click"` в EventBus. Ако добавим нов `QuestSystem`, той просто регистрира своя handler — Button не знае и не го интересува. Всеки subscriber е **независим**.

### Singleton — Какво е и рискът

- **Singleton**: само един екземпляр на класа в цялото приложение. EventBus е Singleton, защото всички компоненти трябва да споделят един и същ bus.
- **Риск**: глобално споделено състояние → трудно тестване, скрити зависимости, race conditions при многонишково изпълнение.

---

## Тема 4: Message Broker

### Queue vs Topic

| | Queue (Point-to-Point) | Topic (Publish-Subscribe) |
|--|--|--|
| Получатели | **Точно един** consumer | **Всички** абонирани consumers |
| Метод | `send()` + `registerConsumer()` + `drain()` | `subscribe()` + `publish()` |
| Употреба | Разпределяне на натоварване | Broadcast |
| Пример | Задача от работна опашка | Нотификация при нова поръчка |

**Правило**: "само до един" → Queue. "до всички" → Topic.

### Шаблон — Producer (Topic режим)

```cpp
class OrderService {
    MessageBroker& broker;
    int nextId = 1;
public:
    explicit OrderService(MessageBroker& broker) : broker(broker) {}

    void placeOrder(const std::string& customer, double total) {
        broker.publish("orders", Message("OrderPlaced", /* payload */));
    }
};
```

### Шаблон — Consumer (Topic режим)

```cpp
class BillingService {
public:
    explicit BillingService(MessageBroker& broker) {
        broker.subscribe("orders", [](const Message& msg) {
            if (msg.type != "OrderPlaced") return;
            std::cout << "[Billing] Фактура за поръчка\n";
        });
    }
};
```

### EventBus vs Message Broker — таблица

| Критерий | EventBus | Message Broker |
|----------|----------|----------------|
| Scope | В един процес | Между процеси/сървъри |
| Трайност | Събитията не се пазят | Съобщенията се буферират |
| Асинхронност | Синхронно | Естествено асинхронно |
| Типична употреба | GUI, игри, вътрешна логика | Микросервизи, системна интеграция |
| Реални системи | — | RabbitMQ, Apache Kafka, AWS SQS |

### Два проблема, които Message Broker решава (срещу директна комуникация)

1. **Tight coupling** — `PaymentService` директно зависи от `ShippingService`, `EmailService` и т.н. С broker — просто публикува и не знае кой слуша.
2. **Синхронност/чупливост** — ако `EmailService` е down, директното извикване се проваля. С broker — съобщението се буферира и се обработва когато service-ът се възстанови.

### Пренаписване с Broker

```cpp
// ПРЕДИ (директни извиквания)
void processPayment(const Order& order) {
    shipping->schedule(order);
    email->sendReceipt(order);
    analytics->record(order);
}

// СЛЕД (с Message Broker)
void processPayment(const Order& order) {
    broker.publish("payments", Message("PaymentMade", order));
}
```

---

## Тема 5: Микроуслуги

### Ключови компоненти

| Компонент | Роля |
|-----------|------|
| **Service Registry** | Каталог с адреси на всички услуги — OrderService пита тук за адреса на UserService |
| **API Gateway** | Единна входна точка — маршрутизира заявките към правилния service |
| **Circuit Breaker** | При много грешки "отваря" прекъсвача и спира заявките към провалена услуга |
| **Message Queue** | Асинхронна комуникация между услуги |

### Circuit Breaker — три състояния

| Състояние | Значение |
|-----------|----------|
| **CLOSED** | Нормален режим — заявките преминават |
| **OPEN** | Твърде много грешки — заявките се блокират (503) |
| **HALF_OPEN** | Тест — пуска се пробна заявка за проверка |

Когато `failureCount >= threshold` → State = **OPEN**.

### Шаблон — API Gateway

```cpp
Response route(const Request& req) {
    if (req.path.starts_with("/users/"))    return forward("UserService", req);
    if (req.path.starts_with("/products/")) return forward("CatalogService", req);
    if (req.path.starts_with("/orders"))    return forward("OrderService", req);
    return {404, "Not found"};
}

Response forward(const std::string& serviceName, const Request& req) {
    if (!registry.isAvailable(serviceName))
        return {503, "Service unavailable"};
    return registry.getService(serviceName)->handle(req);
}
```

### Шаблон — Service (IService имплементация)

```cpp
class ProductService : public IService {
    std::unordered_map<int, Product> catalog = { ... };
public:
    std::string getName() const override { return "ProductService"; }

    Response handle(const Request& req) override {
        if (req.method == "GET" && req.path.starts_with("/products/")) {
            int id = std::stoi(req.path.substr(10));
            auto it = catalog.find(id);
            if (it != catalog.end())
                return {200, "{ \"name\": \"" + it->second.name + "\" }"};
            return {404, "Not found"};
        }
        return {405, "Method not allowed"};
    }
};
```

### Шаблон — OrderService с извикване на други услуги

```cpp
Response createOrder() {
    auto* userSvc = registry.getService("UserService");
    auto userResp = userSvc->handle({"GET", "/users/2", ""});
    if (userResp.statusCode != 200) return {400, "Bad request"};

    auto* catalogSvc = registry.getService("CatalogService");
    auto prodResp = catalogSvc->handle({"GET", "/products/102", ""});
    if (prodResp.statusCode != 200) return {400, "Bad request"};

    std::cout << "[OrderService] Поръчка създадена\n";
    return {201, "Order created"};
}
```

### Предимства и недостатъци на микроуслугите

**Предимства:**
- Независим deployment — промяна в PaymentService не засяга останалите
- Мащабиране само на претоварената услуга
- Изолация на грешки — грешка в един service не сваля системата

**Недостатъци:**
- Мрежова сложност и latency
- Трудно debugging (нужен distributed tracing)
- Сложна инфраструктура (Docker, Kubernetes)

### Кога НЕ да използваме микроуслуги

- Малък екип (< 5 души)
- Прост домейн без ясни граници
- Продуктът е в ранна фаза (неясни изисквания)
- Нямаш опит с distributed systems

### Синхронна vs Асинхронна комуникация

| | Синхронна (HTTP/gRPC) | Асинхронна (Message Queue) |
|--|--|--|
| Принцип | Изпращаш заявка и **чакаш** отговор | Публикуваш и **продължаваш** |
| Проблем | Ако другата услуга е бавна/down — блокираш | Услугата обработва по-късно |
| Кога | Нужен е незабавен отговор | Fire-and-forget, нотификации |

---

## MessageQueue — process() имплементация (Вариант 3 и 5)

```cpp
void process() {
    while (!messages.empty()) {
        auto [topic, msg] = messages.front();
        messages.pop();
        auto it = subscribers.find(topic);
        if (it != subscribers.end())
            for (auto& handler : it->second)
                handler(msg);
    }
}
```

---

## Бързи отговори на теоретични въпроси

**Q: Колко зависимости има Hero от конкретни системи при EventBus подход?**  
A: **Нула** — Hero зависи само от EventBus, не от SoundSystem, HealthBar и т.н.

**Q: Трябва ли да се промени Hero при добавяне на нова система?**  
A: **Не** — новата система просто регистрира свой handler в bus-а. Hero не знае за нея.

**Q: Какво прави EventBus при publish ако няма subscribers?**  
A: Нищо — `handlers.find(event.type)` връща `end()` и не се извиква нищо.

**Q: Защо Model не трябва да знае за View?**  
A: За да може Model да бъде тестван независимо, и за да може да се добавят множество View-та (конзола, HTML, GUI) без промяна на Model.

**Q: Разлика между `send()` и `publish()` в MessageBroker?**  
A: `send()` → Queue (Point-to-Point, **точно един** consumer). `publish()` → Topic (Pub/Sub, **всички** абонати).

---

## Checklist преди контролното

- [ ] Знам как се попълва Observer в Model (`notifyObservers()` след всяка промяна)
- [ ] Знам как се свързват Model и View в Controller конструктора (lambda + `addObserver`)
- [ ] Мога да пиша верига `copy_if → transform → accumulate` за произволна структура
- [ ] Знам разликата Queue vs Topic и кога се използва всеки
- [ ] Мога да напиша `EventEmitter` с `on()` и `emit()` от нулата
- [ ] Мога да напиша `EventBus` Singleton с `subscribe()` и `publish()` от нулата
- [ ] Знам трите състояния на Circuit Breaker и кога се сменят
- [ ] Мога да попълня `route()` и `forward()` на ApiGateway
- [ ] Знам разликата Observer vs EventBus (зависимости)
- [ ] Знам разликата EventBus vs Message Broker (scope, трайност)
