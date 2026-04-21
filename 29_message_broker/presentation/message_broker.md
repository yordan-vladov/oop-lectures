# Message Broker

---

## Съдържание

1. Какво е Message Broker?
2. Проблемите, които решава
3. Основни концепции
4. Имплементация — типове съобщения
5. Имплементация — Message Broker
6. Реален пример: поръчки в онлайн магазин
7. Topic-базирана маршрутизация
8. Event-Driven vs Message Broker — сравнение
9. Обобщение

---

## 1. Какво е Message Broker?

**Message Broker** е посредник, който получава съобщения от **Producer** и ги доставя до **Consumer**.

```
Producer → изпраща съобщение → [Message Broker] → доставя → Consumer
```

> **Принцип**: Producer не знае кой ще обработи съобщението. Consumer не знае кой го е изпратил.

---

### Аналогия — Поща

```
Без Message Broker:              С Message Broker:
A → директно пише на B           A → пуска писмо в пощата
A → директно пише на C                   ↓
A → директно пише на D           Поща сортира и доставя
                                  B, C, D получават писмата
```

- **Producer** = изпращач на писма
- **Broker** = поща (сортира и доставя)
- **Consumer** = получател
- **Queue / Topic** = пощенска кутия

---

## 2. Проблемите, които решава

### Без Message Broker — директна комуникация

```cpp
class OrderService {
    InventoryService* inventory;
    EmailService*     email;
    BillingService*   billing;
    ShippingService*  shipping;

public:
    void placeOrder(const Order& order) {
        inventory->reserve(order.items);   // знае за Inventory
        billing->charge(order.price);      // знае за Billing
        email->sendConfirmation(order);    // знае за Email
        shipping->schedule(order);         // знае за Shipping
    }
};
```

---

### Проблеми:

- **Tight coupling** — OrderService зависи от всички останали
- **Синхронност** — всяко извикване блокира
- **Чупливост** — ако EmailService е down, поръчката се проваля
- **Мащабиране** — нова система → промяна в OrderService

---

### С Message Broker — асинхронна комуникация

```cpp
class OrderService {
    MessageBroker& broker;

public:
    void placeOrder(const Order& order) {
        broker.publish("orders", Message("OrderPlaced", order));
        // OrderService свободен — не чака отговор
    }
};

// Независими консюмери реагират по своя график
// InventoryService слуша "orders"
// BillingService слуша "orders"
// EmailService слуша "orders"
```

---

## 3. Основни концепции

| Концепция       | Роля                                                        |
|-----------------|-------------------------------------------------------------|
| **Message**     | Данните, които се предават (тип + payload)                  |
| **Producer**    | Компонентът, който изпраща съобщения                        |
| **Consumer**    | Компонентът, който получава и обработва съобщения           |
| **Queue**       | Опашка — съобщението се доставя до **един** consumer        |
| **Topic**       | Тема — съобщението се доставя до **всички** абонати         |
| **Broker**      | Посредникът, който управлява queues и topics                |

---

### Queue (Point-to-Point)

Съобщението се доставя до **точно един** Consumer — подходящо за разпределение на натоварването.

```
Producer → [Queue] → Consumer1
           (само един получава)
```

---

### Topic (Publish-Subscribe)

Съобщението се доставя до **всички абонирани** Consumer-и — подходящо за broadcast.

```
Producer → [Topic] → Consumer1
                   → Consumer2
                   → Consumer3
              (всички получават)
```

---

## 4. Имплементация — типове съобщения

```cpp
#pragma once
#include <string>
#include <any>
#include <chrono>

struct Message {
    std::string id;
    std::string type;
    std::any    payload;
    std::chrono::system_clock::time_point timestamp;

    Message(const std::string& type, std::any payload)
        : type(type),
          payload(std::move(payload)),
          timestamp(std::chrono::system_clock::now())
    {
        // генерираме прост id от типа и времето
        id = type + "_" + std::to_string(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                timestamp.time_since_epoch()
            ).count()
        );
    }
};
```

---

## 5. Имплементация — Message Broker

```cpp
#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <queue>
#include <vector>
#include <iostream>
#include "Message.h"

class MessageBroker {
public:
    using Handler = std::function<void(const Message&)>;

private:
    // Topic → списък от handler-и (Publish-Subscribe)
    std::unordered_map<std::string, std::vector<Handler>> topics;

    // Queue name → опашка от съобщения (Point-to-Point)
    std::unordered_map<std::string, std::queue<Message>> queues;

    // Queue name → регистриран consumer
    std::unordered_map<std::string, Handler> queueConsumers;

public:
    // --- Topic (Pub/Sub) ---
    void subscribe(const std::string& topic, Handler handler) {
        topics[topic].push_back(std::move(handler));
    }

    void publish(const std::string& topic, const Message& msg) {
        auto it = topics.find(topic);
        if (it != topics.end()) {
            for (auto& handler : it->second) {
                handler(msg);
            }
        }
    }

    // --- Queue (Point-to-Point) ---
    void registerConsumer(const std::string& queueName, Handler handler) {
        queueConsumers[queueName] = std::move(handler);
    }

    void send(const std::string& queueName, const Message& msg) {
        queues[queueName].push(msg);
    }

    // "Изпомпва" съобщенията от опашката към consumer-а
    void drain(const std::string& queueName) {
        auto& q = queues[queueName];
        auto  it = queueConsumers.find(queueName);
        if (it == queueConsumers.end()) {
            std::cerr << "[Broker] Няма регистриран consumer за: " << queueName << "\n";
            return;
        }
        while (!q.empty()) {
            it->second(q.front());
            q.pop();
        }
    }
};
```

---

## 6. Реален пример: поръчки в онлайн магазин

### Дефиниция на домейна

```cpp
#pragma once
#include <string>
#include <vector>

struct OrderItem {
    std::string name;
    int         quantity;
    double      price;
};

struct Order {
    int                    id;
    std::string            customer;
    std::vector<OrderItem> items;
    double                 total;
};
```

---

### Producer — OrderService

```cpp
#pragma once
#include "MessageBroker.h"
#include "Order.h"
#include <iostream>

class OrderService {
    MessageBroker& broker;
    int nextId = 1;

public:
    explicit OrderService(MessageBroker& broker) : broker(broker) {}

    void placeOrder(const std::string& customer,
                    const std::vector<OrderItem>& items)
    {
        double total = 0;
        for (auto& item : items) total += item.quantity * item.price;

        Order order{nextId++, customer, items, total};

        std::cout << "[OrderService] Нова поръчка #" << order.id
                  << " от " << customer << " — " << total << " лв.\n";

        // Публикуваме в topic "orders" — всички абонати ще получат
        broker.publish("orders", Message("OrderPlaced", order));
    }
};
```

---

### Consumers — независими сервизи

```cpp
#pragma once
#include "MessageBroker.h"
#include "Order.h"
#include <iostream>
#include <any>

class InventoryService {
public:
    explicit InventoryService(MessageBroker& broker) {
        broker.subscribe("orders", [](const Message& msg) {
            if (msg.type != "OrderPlaced") return;
            auto order = std::any_cast<Order>(msg.payload);
            std::cout << "[Inventory] Резервиране на стоки за поръчка #"
                      << order.id << "\n";
            for (auto& item : order.items)
                std::cout << "  - " << item.quantity << "x " << item.name << "\n";
        });
    }
};

class BillingService {
public:
    explicit BillingService(MessageBroker& broker) {
        broker.subscribe("orders", [](const Message& msg) {
            if (msg.type != "OrderPlaced") return;
            auto order = std::any_cast<Order>(msg.payload);
            std::cout << "[Billing] Таксуване на " << order.customer
                      << " — " << order.total << " лв.\n";
        });
    }
};

class EmailService {
public:
    explicit EmailService(MessageBroker& broker) {
        broker.subscribe("orders", [](const Message& msg) {
            if (msg.type != "OrderPlaced") return;
            auto order = std::any_cast<Order>(msg.payload);
            std::cout << "[Email] Изпращане на потвърждение до "
                      << order.customer << "\n";
        });
    }
};
```

---

### main.cpp

```cpp
#include "MessageBroker.h"
#include "OrderService.h"
#include "InventoryService.h"
#include "BillingService.h"
#include "EmailService.h"

int main() {
    MessageBroker broker;

    // Абонираме независимите сервизи
    InventoryService inventory(broker);
    BillingService   billing(broker);
    EmailService     email(broker);

    // OrderService само публикува — не знае за останалите
    OrderService orders(broker);

    orders.placeOrder("Иван", {
        {"Клавиатура", 1, 79.99},
        {"Мишка",      2, 24.99}
    });

    std::cout << "---\n";

    orders.placeOrder("Мария", {
        {"Монитор", 1, 399.00}
    });

    return 0;
}
```

---

### Изход

```
[OrderService] Нова поръчка #1 от Иван — 129.97 лв.
[Inventory] Резервиране на стоки за поръчка #1
  - 1x Клавиатура
  - 2x Мишка
[Billing] Таксуване на Иван — 129.97 лв.
[Email] Изпращане на потвърждение до Иван
---
[OrderService] Нова поръчка #2 от Мария — 399.00 лв.
[Inventory] Резервиране на стоки за поръчка #2
  - 1x Монитор
[Billing] Таксуване на Мария — 399.00 лв.
[Email] Изпращане на потвърждение до Мария
```

> OrderService публикува веднъж — три независими сервиза реагират.

---

## 7. Topic-базирана маршрутизация

При по-сложни системи различни типове съобщения се маршрутизират към различни topics.

---

### Множество topics

```cpp
class ShippingService {
public:
    explicit ShippingService(MessageBroker& broker) {
        // Слуша само платени поръчки
        broker.subscribe("payments", [](const Message& msg) {
            if (msg.type != "PaymentConfirmed") return;
            auto order = std::any_cast<Order>(msg.payload);
            std::cout << "[Shipping] Изпращане на поръчка #"
                      << order.id << "\n";
        });
    }
};

class FraudDetectionService {
public:
    explicit FraudDetectionService(MessageBroker& broker) {
        // Слуша всички плащания за анализ
        broker.subscribe("payments", [](const Message& msg) {
            auto order = std::any_cast<Order>(msg.payload);
            std::cout << "[Fraud] Анализ на плащане: "
                      << order.total << " лв.\n";
        });
    }
};
```

---

### Разделяне на отговорностите по topics

```
"orders"   → InventoryService, BillingService, EmailService
"payments" → ShippingService, FraudDetectionService, AuditService
"errors"   → AlertService, LoggingService
```

Всеки сервиз абонира само topics, **релевантни за него** — принцип на минималната зависимост (ISP от SOLID).

---

## 8. Event-Driven vs Message Broker — сравнение

| Критерий              | Event-Driven (EventBus)          | Message Broker                        |
|-----------------------|----------------------------------|---------------------------------------|
| **Scope**             | В рамките на един процес         | Между процеси / сървъри               |
| **Асинхронност**      | Обикновено синхронно             | Естествено асинхронно                 |
| **Трайност**          | Събитията не се пазят            | Съобщенията се буферират в опашки     |
| **Мащабиране**        | В рамките на приложението        | Хоризонтално — много инстанции        |
| **Примери**           | GUI, игри, вътрешна логика       | Микросървиси, системна интеграция     |
| **Реални системи**    | —                                | RabbitMQ, Apache Kafka, AWS SQS       |

---

### Кога да избереш кое?

```
EventBus      → компонентите са в един процес, нужна е бърза реакция
                (GUI обновления, игрова логика)

Message Broker → различни сервизи / процеси трябва да комуникират
                надеждно и асинхронно
                (поръчки, плащания, нотификации)
```

---

## Обобщение

```
Message Broker = надежден посредник за асинхронна комуникация

Producer  →  изпраща Message  →  [Broker]
                                     ↓
Consumer  ←  получава Message  ←  Queue / Topic

Два режима:
  Queue  → точно един consumer получава (балансиране на натоварване)
  Topic  → всички абонати получават    (broadcast)

ООП принципи в действие:
  ✓ Абстракция     — Producer не знае как работи Broker
  ✓ Loose coupling — сервизите не зависят един от друг
  ✓ Single Responsibility — всеки сервиз прави само своята работа
  ✓ Open/Closed   — нов Consumer без промяна на Producer
```
