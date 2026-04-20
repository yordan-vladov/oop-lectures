# Микроуслуги (Microservices)

---

## Съдържание

1. Какво са Микроуслугите?
2. Проблемите, които решават
3. Основни концепции
4. Прост пример на C++
5. Комуникация между услуги
6. Предимства и недостатъци
7. Монолит vs Микроуслуги - сравнение
8. Обобщение

---

## 1. Какво са Микроуслугите?

**Микроуслугите** са архитектурен стил, при който приложението е разделено на малки, **независими услуги**, всяка от които:

- Изпълнява **една конкретна бизнес функция**
- Работи в **собствен процес**
- Комуникира с другите услуги чрез **мрежа** (HTTP, gRPC, опашки от съобщения)
- Може да бъде **разгърната независимо**

> **Принцип**: Всяка услуга прави едно нещо и го прави добре.

---

![](https://microservices.io/i/Microservice_Architecture.png)

---

### Аналогия

![](https://blog.nashtechglobal.com/wp-content/uploads/2025/12/frame-1-4.png)

---

## 2. Проблемите, които решават

- Без микроуслуги трябва да разчитаме на монолитна архитектура, където цялото приложение се пуска като един процес.
- Този подход работи при по-малки приложения, но при по-голям мащаб започва да създава проблеми.

---

### Проблеми с монолита:

- **Мащабиране** - трябва да мащабираш целия монолит, дори само един модул е натоварен
- **Deployment** - малка промяна в Плащанията изисква нов deploy на цялото приложение
- **Технологичен lock-in** - всичко трябва да е на един език и framework
- **Екипна работа** - множество екипи работят в една кодова база → конфликти
- **Рискове** - грешка в един модул може да срине цялото приложение

---

```cpp
// МОНОЛИТ - всичко е свързано в един клас/процес
class ECommerceApp {
    UserService    users;
    OrderService   orders;
    PaymentService payments;
    CatalogService catalog;

public:
    void placeOrder(int userId, int productId) {
        auto user    = users.getUser(userId);      // директно извикване
        auto product = catalog.getProduct(productId);
        auto order   = orders.createOrder(user, product);
        payments.charge(user.card, product.price); // директна зависимост
    }
};
```

---

### С микроуслуги - независими компоненти

```
Преди (монолит):                  След (микроуслуги):
Промяна в Плащания                Промяна в Плащания
       ↓                                 ↓
Deploy на целия сайт              Deploy само на PaymentService
(30 минути, риск за всичко)       (2 минути, само тази услуга)
```

- Всяка услуга се мащабира **отделно**
- Всяка услуга може да е написана на **различен език**
- Грешка в едната услуга **не сваля** останалите
- Малки екипи работят **независимо**

---

## 3. Основни концепции

| Концепция              | Описание                                                              |
| ---------------------- | --------------------------------------------------------------------- |
| **Service**            | Независим компонент с ясна бизнес отговорност                        |
| **API Gateway**        | Единна входна точка, разпределя заявките към услугите                |
| **Service Registry**   | Каталог с адресите на всички услуги                                  |
| **Message Queue**      | Опашка за асинхронна комуникация между услуги                        |
| **Load Balancer**      | Разпределя трафика между множество инстанции на една услуга          |
| **Circuit Breaker**    | Спира заявките към провалена услуга, за да предпази системата        |

---

### Два вида комуникация

```
Синхронна (HTTP/gRPC):           Асинхронна (Message Queue):
  Client                           OrderService
     │                                  │
     ▼                             публикува "OrderPlaced"
  OrderService ──────► PaymentService        │
  чака отговор                     ▼    ▼    ▼
     │                         Email  Stock  Analytics
     ◄──────────── отговор
```

---

## 4. Прост пример на C++

**Сценарий**: Система за поръчки с три услуги: `UserService`, `CatalogService`, `OrderService`.

---

### Интерфейс за услуга - `IService.h`

```cpp
#pragma once
#include <string>
#include <unordered_map>
#include <functional>

// Симулираме HTTP заявка/отговор
struct Request {
    std::string method;   // "GET", "POST"
    std::string path;
    std::string body;
};

struct Response {
    int statusCode;       // 200, 404, 500
    std::string body;
};

// Всяка услуга имплементира този интерфейс
class IService {
public:
    virtual std::string getName() const = 0;
    virtual Response handle(const Request& req) = 0;
    virtual ~IService() = default;
};
```

---

### UserService - `UserService.h`

```cpp
#pragma once
#include "IService.h"
#include <unordered_map>

class UserService : public IService {
    struct User { int id; std::string name; std::string email; };
    std::unordered_map<int, User> users = {
        {1, {1, "Алис", "alice@example.com"}},
        {2, {2, "Боби", "bob@example.com"}},
    };

public:
    std::string getName() const override { return "UserService"; }

    Response handle(const Request& req) override {
        if (req.method == "GET" && req.path.starts_with("/users/")) {
            int id = std::stoi(req.path.substr(7));
            auto it = users.find(id);
            if (it != users.end()) {
                return {200, "{ \"id\": " + std::to_string(it->second.id) +
                             ", \"name\": \"" + it->second.name + "\" }"};
            }
            return {404, "{ \"error\": \"User not found\" }"};
        }
        return {405, "{ \"error\": \"Method not allowed\" }"};
    }
};
```

---

### CatalogService - `CatalogService.h`

```cpp
#pragma once
#include "IService.h"
#include <unordered_map>

class CatalogService : public IService {
    struct Product { int id; std::string name; double price; int stock; };
    std::unordered_map<int, Product> products = {
        {101, {101, "Лаптоп",  1500.00, 10}},
        {102, {102, "Мишка",     25.99, 50}},
        {103, {103, "Клавиатура", 79.99,  0}},
    };

public:
    std::string getName() const override { return "CatalogService"; }

    Response handle(const Request& req) override {
        if (req.method == "GET" && req.path.starts_with("/products/")) {
            int id = std::stoi(req.path.substr(10));
            auto it = products.find(id);
            if (it != products.end()) {
                const auto& p = it->second;
                return {200, "{ \"id\": " + std::to_string(p.id) +
                             ", \"name\": \"" + p.name +
                             "\", \"price\": " + std::to_string(p.price) +
                             ", \"stock\": " + std::to_string(p.stock) + " }"};
            }
            return {404, "{ \"error\": \"Product not found\" }"};
        }
        return {405, "{ \"error\": \"Method not allowed\" }"};
    }
};
```

---

### Service Registry - `ServiceRegistry.h`

```cpp
#pragma once
#include "IService.h"
#include <unordered_map>
#include <memory>
#include <stdexcept>

// Регистър на всички услуги - знае кой е достъпен и на какъв "адрес"
class ServiceRegistry {
    std::unordered_map<std::string, IService*> services;

public:
    void registerService(IService* service) {
        services[service->getName()] = service;
        std::cout << "[Registry] Регистрирана: " << service->getName() << "\n";
    }

    IService* getService(const std::string& name) {
        auto it = services.find(name);
        if (it == services.end()) {
            throw std::runtime_error("Услугата не е намерена: " + name);
        }
        return it->second;
    }

    bool isAvailable(const std::string& name) const {
        return services.count(name) > 0;
    }
};
```

---

### OrderService - `OrderService.h`

```cpp
#pragma once
#include "IService.h"
#include "ServiceRegistry.h"
#include <sstream>

class OrderService : public IService {
    ServiceRegistry& registry;
    int nextOrderId = 1;

public:
    explicit OrderService(ServiceRegistry& reg) : registry(reg) {}
    std::string getName() const override { return "OrderService"; }

    Response handle(const Request& req) override {
        if (req.method == "POST" && req.path == "/orders") {
            return createOrder(req.body);
        }
        return {405, "{ \"error\": \"Method not allowed\" }"};
    }

private:
    Response createOrder(const std::string& body) {
        // Симулираме парсване на "userId=1&productId=101"
        int userId = 1, productId = 101;

        // Извикване на UserService
        auto* userSvc = registry.getService("UserService");
        auto userResp = userSvc->handle({"GET", "/users/" + std::to_string(userId), ""});
        if (userResp.statusCode != 200) {
            return {400, "{ \"error\": \"Невалиден потребител\" }"};
        }

        // Извикване на CatalogService
        auto* catalogSvc = registry.getService("CatalogService");
        auto prodResp = catalogSvc->handle({"GET", "/products/" + std::to_string(productId), ""});
        if (prodResp.statusCode != 200) {
            return {400, "{ \"error\": \"Продуктът не е намерен\" }"};
        }

        int orderId = nextOrderId++;
        std::cout << "[OrderService] Създадена поръчка #" << orderId << "\n";
        std::cout << "  Потребител: " << userResp.body << "\n";
        std::cout << "  Продукт: "   << prodResp.body << "\n";

        return {201, "{ \"orderId\": " + std::to_string(orderId) + ", \"status\": \"created\" }"};
    }
};
```

---

### API Gateway - `ApiGateway.h`

```cpp
#pragma once
#include "ServiceRegistry.h"
#include <iostream>

// Единна входна точка - разпределя заявките
class ApiGateway {
    ServiceRegistry& registry;

public:
    explicit ApiGateway(ServiceRegistry& reg) : registry(reg) {}

    Response route(const Request& req) {
        std::cout << "[Gateway] " << req.method << " " << req.path << "\n";

        // Routing: /users/* → UserService, /products/* → CatalogService, /orders → OrderService
        if (req.path.starts_with("/users/"))    return forward("UserService", req);
        if (req.path.starts_with("/products/")) return forward("CatalogService", req);
        if (req.path.starts_with("/orders"))    return forward("OrderService", req);

        return {404, "{ \"error\": \"Маршрутът не е намерен\" }"};
    }

private:
    Response forward(const std::string& serviceName, const Request& req) {
        if (!registry.isAvailable(serviceName)) {
            return {503, "{ \"error\": \"Услугата не е достъпна\" }"};
        }
        return registry.getService(serviceName)->handle(req);
    }
};
```

---

### `main.cpp`

```cpp
#include "UserService.h"
#include "CatalogService.h"
#include "OrderService.h"
#include "ServiceRegistry.h"
#include "ApiGateway.h"
#include <iostream>

int main() {
    // 1. Инициализиране на услугите
    ServiceRegistry registry;

    UserService    userService;
    CatalogService catalogService;
    OrderService   orderService(registry);

    // 2. Регистриране в Service Registry
    registry.registerService(&userService);
    registry.registerService(&catalogService);
    registry.registerService(&orderService);

    // 3. API Gateway - единна точка за достъп
    ApiGateway gateway(registry);

    std::cout << "\n--- Заявки към системата ---\n\n";

    // Заявка за потребител
    auto r1 = gateway.route({"GET", "/users/1", ""});
    std::cout << "Отговор [" << r1.statusCode << "]: " << r1.body << "\n\n";

    // Заявка за продукт
    auto r2 = gateway.route({"GET", "/products/101", ""});
    std::cout << "Отговор [" << r2.statusCode << "]: " << r2.body << "\n\n";

    // Създаване на поръчка (OrderService извиква UserService и CatalogService)
    auto r3 = gateway.route({"POST", "/orders", "userId=1&productId=101"});
    std::cout << "Отговор [" << r3.statusCode << "]: " << r3.body << "\n\n";

    // Несъществуващ маршрут
    auto r4 = gateway.route({"GET", "/unknown", ""});
    std::cout << "Отговор [" << r4.statusCode << "]: " << r4.body << "\n";

    return 0;
}
```

---

### Изходен резултат

```
[Registry] Регистрирана: UserService
[Registry] Регистрирана: CatalogService
[Registry] Регистрирана: OrderService

--- Заявки към системата ---

[Gateway] GET /users/1
Отговор [200]: { "id": 1, "name": "Алис" }

[Gateway] GET /products/101
Отговор [200]: { "id": 101, "name": "Лаптоп", "price": 1500.000000, "stock": 10 }

[Gateway] POST /orders
[OrderService] Създадена поръчка #1
  Потребител: { "id": 1, "name": "Алис" }
  Продукт: { "id": 101, "name": "Лаптоп", "price": 1500.000000, "stock": 10 }
Отговор [201]: { "orderId": 1, "status": "created" }

[Gateway] GET /unknown
Отговор [404]: { "error": "Маршрутът не е намерен" }
```

---

## 5. Комуникация между услуги

### Синхронна комуникация (HTTP/gRPC)

Услугата изпраща заявка и **чака отговор**.

```
OrderService ──── GET /products/101 ────► CatalogService
             ◄─── { "price": 1500 } ─────
```

**Проблем**: Ако CatalogService е бавен или недостъпен, OrderService също блокира.

---

### Асинхронна комуникация (Message Queue)

Услугата публикува съобщение и **не чака** - другите услуги го обработват по-късно.

```cpp
// Опростена симулация на Message Queue
class MessageQueue {
    using Handler = std::function<void(const std::string&)>;
    std::unordered_map<std::string, std::vector<Handler>> subscribers;
    std::queue<std::pair<std::string, std::string>> messages;

public:
    void subscribe(const std::string& topic, Handler handler) {
        subscribers[topic].push_back(handler);
    }

    void publish(const std::string& topic, const std::string& message) {
        std::cout << "[Queue] Публикувано в '" << topic << "': " << message << "\n";
        messages.push({topic, message});
    }

    void process() {
        while (!messages.empty()) {
            auto [topic, msg] = messages.front();
            messages.pop();
            for (auto& handler : subscribers[topic]) {
                handler(msg);
            }
        }
    }
};
```

---

### Пример: OrderService публикува, EmailService слуша

```cpp
int main() {
    MessageQueue queue;

    // EmailService се абонира
    queue.subscribe("order.created", [](const std::string& msg) {
        std::cout << "[EmailService] Изпращане на потвърждение: " << msg << "\n";
    });

    // StockService се абонира
    queue.subscribe("order.created", [](const std::string& msg) {
        std::cout << "[StockService] Намаляване на наличността за: " << msg << "\n";
    });

    // AnalyticsService се абонира
    queue.subscribe("order.created", [](const std::string& msg) {
        std::cout << "[Analytics] Записване на продажба: " << msg << "\n";
    });

    // OrderService публикува - не знае кой слуша
    queue.publish("order.created", "orderId=1, product=Лаптоп, user=Алис");

    // Обработваме опашката (в реалност - асинхронно)
    queue.process();

    return 0;
}
```

---

### Изход

```
[Queue] Публикувано в 'order.created': orderId=1, product=Лаптоп, user=Алис
[EmailService] Изпращане на потвърждение: orderId=1, product=Лаптоп, user=Алис
[StockService] Намаляване на наличността за: orderId=1, product=Лаптоп, user=Алис
[Analytics] Записване на продажба: orderId=1, product=Лаптоп, user=Алис
```

> OrderService не знае нищо за Email, Stock или Analytics - само публикува събитие.

---

### Circuit Breaker - предпазва системата

```cpp
// При много грешки "отваряме" прекъсвача и спираме заявките
class CircuitBreaker {
    enum class State { CLOSED, OPEN, HALF_OPEN };
    State state = State::CLOSED;
    int failureCount = 0;
    const int threshold = 3;

public:
    Response call(const std::string& serviceName,
                  std::function<Response()> action) {
        if (state == State::OPEN) {
            std::cout << "[CircuitBreaker] " << serviceName << " е недостъпна!\n";
            return {503, "{ \"error\": \"Service unavailable\" }"};
        }

        try {
            auto response = action();
            if (response.statusCode >= 500) {
                recordFailure(serviceName);
            } else {
                failureCount = 0; // успешна заявка → нулираме брояча
                state = State::CLOSED;
            }
            return response;
        } catch (...) {
            recordFailure(serviceName);
            return {503, "{ \"error\": \"Service error\" }"};
        }
    }

private:
    void recordFailure(const std::string& name) {
        failureCount++;
        std::cout << "[CircuitBreaker] Грешка #" << failureCount << " в " << name << "\n";
        if (failureCount >= threshold) {
            state = State::OPEN;
            std::cout << "[CircuitBreaker] Прекъсвачът е ОТВОРЕН за " << name << "\n";
        }
    }
};
```

---

## 6. Предимства и недостатъци

### Предимства

```
✓ Независим deployment  - промяна в PaymentService не засяга UserService
✓ Мащабиране           - мащабираш само претоварената услуга
✓ Устойчивост          - грешка в една услуга не сваля системата
✓ Технологична свобода - всяка услуга може да е на различен език
✓ Малки екипи          - всеки екип притежава своята услуга
✓ По-лесно разбиране   - малка кодова база за всяка услуга
```

---

### Недостатъци

```
✗ Мрежова сложност     - комуникацията по мрежа е по-бавна и ненадеждна
✗ Distributed трудности - транзакции, consistency, debugging
✗ Инфраструктура       - нужен е Kubernetes, Docker, Service Mesh
✗ Latency              - всяко извикване между услуги добавя забавяне
✗ Тестване             - интеграционните тестове са по-сложни
✗ Overhead             - за малки приложения е прекалено
```

---

### Кога да НЕ използваш микроуслуги?

```
✗ Малък екип (< 5 души)
✗ Прост домейн без ясни граници между модулите
✗ Нямаш опит с distributed systems
✗ Продуктът е в ранна фаза (неясни изисквания)

→ Започни с монолит. Мигрирай към микроуслуги когато болката е реална.
```

---

## 7. Монолит vs Микроуслуги - Сравнение

| Критерий               | Монолит                             | Микроуслуги                              |
| ---------------------- | ----------------------------------- | ---------------------------------------- |
| **Deployment**         | Целият проект наведнъж              | Всяка услуга независимо                  |
| **Мащабиране**         | Цялото приложение                   | Само нужните услуги                      |
| **Грешки**             | Могат да засегнат всичко            | Изолирани в услугата                     |
| **Комуникация**        | Директни извиквания (бързо)         | Мрежа (по-бавно, ненадеждно)             |
| **Технологии**         | Един стек                           | Всяка услуга избира свой                 |
| **Тестване**           | По-лесно (локално)                  | По-сложно (интеграционни тестове)        |
| **Debugging**          | Прост стек трейс                    | Distributed tracing нужен                |
| **Подходящ за**        | Малки/средни проекти, MVP           | Големи системи, много екипи              |
| **Инфраструктура**     | Прост сървър                        | Docker, Kubernetes, CI/CD за всяка услуга |

---

### Кога да избереш кое?

```
Монолит → стартиращ проект, малък екип, прост домейн
          (Netflix, Amazon, Uber започнаха като монолити!)

Микроуслуги → голям екип (10+ души), ясни бизнес граници,
              различни изисквания за мащабиране на модулите,
              нужда от независими deployments
```

---

## Обобщение

```
Микроуслуги = приложение разделено на малки независими услуги

Ключови компоненти:
  Service Registry  → каталог с адресите на услугите
  API Gateway       → единна входна точка, routing
  Message Queue     → асинхронна комуникация
  Circuit Breaker   → предпазва при провал на услуга

Комуникация:
  Синхронна  → HTTP/gRPC (чакаме отговор)
  Асинхронна → Message Queue (публикуваме и продължаваме)

Предимства:
  ✓ Независим deployment и мащабиране
  ✓ Изолация на грешки
  ✓ Технологична свобода

Недостатъци:
  ✗ Мрежова сложност и latency
  ✗ Сложна инфраструктура
  ✗ Трудно debugging

Принцип:
  Започни с монолит. Мигрирай когато болката оправдава цената.
```
