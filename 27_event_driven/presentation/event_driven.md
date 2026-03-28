# Event-Driven Design

---

## Съдържание

1. Какво е Event-Driven Design?
2. Проблемите, които решава
3. Основни концепции
4. Прост пример — директни callbacks
5. Event Bus — централизирана система
6. Реален пример: игра с герой
7. Observer vs Event-Driven — сравнение
8. Резюме

---

## 1. Какво е Event-Driven Design?

**Event-Driven Design** е архитектурен подход, при който компонентите **не се извикват директно**.

Вместо това:
- Компонент **публикува събитие** ("нещо се случи")
- Друг компонент **слуша** за събитието и реагира

> **Принцип**: Компонентите не знаят един за друг — знаят само за събитията.

---

### Аналогия

```
Без EDD:                          С EDD:
Button → извиква → Logger         Button → публикува → "ButtonClicked"
Button → извиква → Validator                               ↓
Button → извиква → UIUpdater      Logger слуша → реагира
                                  Validator слуша → реагира
                                  UIUpdater слуша → реагира
```

---

## 2. Проблемите, които решава

### Без Event-Driven — твърди зависимости

```cpp
class Button {
    Logger* logger;
    Validator* validator;
    UIUpdater* updater;

public:
    Button(Logger* l, Validator* v, UIUpdater* u)
        : logger(l), validator(v), updater(u) {}

    void click() {
        logger->log("clicked");       // Button знае за Logger
        validator->validate();        // Button знае за Validator
        updater->refresh();           // Button знае за UIUpdater
    }
};
```

---

### Проблеми:

- **Tight coupling** — Button зависи от конкретни класове
- **Трудно разширяване** — нов слушател → промяна в Button
- **Трудно тестване** — трябват mock-ове за всяка зависимост
- **Ред на изпълнение** — Button управлява всичко

---

### С Event-Driven — слаби зависимости

```cpp
button.on("click", [](const Event& e) { logger.log("clicked"); });
button.on("click", [](const Event& e) { validator.validate(); });
button.on("click", [](const Event& e) { updater.refresh(); });

button.click(); // публикува "click" → всички слушатели реагират
```

- Button не знае кой слуша
- Нов слушател — само добавяш `on(...)`, без промяна на Button

---

## 3. Основни концепции

| Концепция        | Роля                                              |
|------------------|---------------------------------------------------|
| **Event**        | Обект, описващ "какво се е случило"               |
| **Publisher**    | Компонентът, който публикува събитие              |
| **Subscriber**   | Компонентът, който слуша и реагира                |
| **Event Bus**    | Централен посредник — свързва Publisher и Subscriber |
| **Handler**      | Функцията, която се изпълнява при събитие         |

---

## 4. Прост пример — директни callbacks

### Дефиниране на събитие

```cpp
#pragma once
#include <string>
#include <any>

struct Event {
    std::string type;   // "click", "keypress", "playerDied", ...
    std::any data;      // допълнителни данни (по избор)

    Event(const std::string& type) : type(type) {}
    Event(const std::string& type, std::any data)
        : type(type), data(data) {}
};
```

---

### EventEmitter — обект, който може да публикува

```cpp
#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include "Event.h"

class EventEmitter {
    using Handler = std::function<void(const Event&)>;
    std::unordered_map<std::string, std::vector<Handler>> handlers;

public:
    // Регистрира слушател за събитие
    void on(const std::string& eventType, Handler handler) {
        handlers[eventType].push_back(handler);
    }

    // Публикува събитие — извиква всички слушатели
    void emit(const Event& event) {
        auto it = handlers.find(event.type);
        if (it != handlers.end()) {
            for (auto& handler : it->second) {
                handler(event);
            }
        }
    }
};
```

---

### Употреба

```cpp
#include "EventEmitter.h"
#include <iostream>

int main() {
    EventEmitter emitter;

    // Регистрираме слушатели
    emitter.on("click", [](const Event& e) {
        std::cout << "[Logger] Бутонът е натиснат\n";
    });

    emitter.on("click", [](const Event& e) {
        std::cout << "[UI] Обновяване на интерфейса\n";
    });

    emitter.on("hover", [](const Event& e) {
        std::cout << "[UI] Показване на tooltip\n";
    });

    // Публикуваме събитие
    emitter.emit(Event("click")); // → извиква Logger и UI
    emitter.emit(Event("hover")); // → извиква само tooltip

    return 0;
}
```

---

### Изход

```
[Logger] Бутонът е натиснат
[UI] Обновяване на интерфейса
[UI] Показване на tooltip
```

---

## 5. Event Bus — централизирана система

При по-голямо приложение всеки компонент да има свой EventEmitter е неудобно.

**Event Bus** = глобален (или инжектиран) посредник за всички събития.

---

### EventBus имплементация

```cpp
#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include "Event.h"

class EventBus {
    using Handler = std::function<void(const Event&)>;
    std::unordered_map<std::string, std::vector<Handler>> handlers;

public:
    // Единствен екземпляр (Singleton)
    static EventBus& getInstance() {
        static EventBus instance;
        return instance;
    }

    void subscribe(const std::string& eventType, Handler handler) {
        handlers[eventType].push_back(handler);
    }

    void publish(const Event& event) {
        auto it = handlers.find(event.type);
        if (it != handlers.end()) {
            for (auto& handler : it->second) {
                handler(event);
            }
        }
    }

private:
    EventBus() = default;
};
```

---

### Употреба с EventBus

```cpp
// Компонент A — публикува, не знае кой слуша
class LoginForm {
public:
    void submit(const std::string& username) {
        std::cout << "[LoginForm] Изпращане...\n";
        EventBus::getInstance().publish(Event("userLoggedIn", username));
    }
};

// Компонент B — слуша, не знае кой публикува
class Dashboard {
public:
    Dashboard() {
        EventBus::getInstance().subscribe("userLoggedIn", [](const Event& e) {
            auto user = std::any_cast<std::string>(e.data);
            std::cout << "[Dashboard] Добре дошъл, " << user << "!\n";
        });
    }
};

// Компонент C — независим слушател
class AuditLog {
public:
    AuditLog() {
        EventBus::getInstance().subscribe("userLoggedIn", [](const Event& e) {
            auto user = std::any_cast<std::string>(e.data);
            std::cout << "[AuditLog] Вход: " << user << "\n";
        });
    }
};
```

---

### main.cpp

```cpp
#include "EventBus.h"
#include <iostream>

int main() {
    Dashboard dashboard;  // регистрира слушател
    AuditLog auditLog;    // регистрира слушател

    LoginForm form;
    form.submit("Мария"); // публикува "userLoggedIn"

    return 0;
}
```

---

### Изход

```
[LoginForm] Изпращане...
[Dashboard] Добре дошъл, Мария!
[AuditLog] Вход: Мария
```

> LoginForm не знае нищо за Dashboard или AuditLog.

---

## 6. Реален пример: игра с герой

**Сценарий**: Герой получава щети — много системи трябва да реагират.

---

### Без Event-Driven

```cpp
class Hero {
    HealthBar* healthBar;
    SoundSystem* sound;
    AchievementSystem* achievements;
    GameOverScreen* gameOver;

public:
    void takeDamage(int amount) {
        hp -= amount;
        healthBar->update(hp);          // Hero знае за UI
        sound->play("hurt");            // Hero знае за Sound
        if (hp <= 0) {
            achievements->unlock("FirstDeath");
            gameOver->show();           // Hero знае за GameOver
        }
    }
};
```

Всяка нова система → промяна в Hero.

---

### С Event-Driven

```cpp
class Hero {
    EventBus& bus;
    int hp = 100;

public:
    Hero(EventBus& bus) : bus(bus) {}

    void takeDamage(int amount) {
        hp -= amount;
        bus.publish(Event("heroDamaged", hp));
        if (hp <= 0) {
            bus.publish(Event("heroDied"));
        }
    } 	
5 	Математика 	
};
```

---

```cpp
class HealthBar {
public:
    HealthBar(EventBus& bus) {
        bus.subscribe("heroDamaged", [](const Event& e) {
            int hp = std::any_cast<int>(e.data);
            std::cout << "[HealthBar] HP: " << hp << "\n";
        });
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

class GameOverScreen {
public:
    GameOverScreen(EventBus& bus) {
        bus.subscribe("heroDied", [](const Event& e) {
            std::cout << "[GameOver] Край на играта!\n";
        });
    }
};
```

---

### main.cpp

```cpp
int main() {
    EventBus bus;

    HealthBar healthBar(bus);
    SoundSystem sound(bus);
    GameOverScreen gameOver(bus);

    Hero hero(bus);
    hero.takeDamage(30);
    hero.takeDamage(80);

    return 0;
}
```

---

### Изход

```
[HealthBar] HP: 70
[Sound] hurt.wav
[HealthBar] HP: -10
[Sound] hurt.wav
[Sound] death.wav
[GameOver] Край на играта!
```

> Hero публикува — не знае кой реагира. Можем да добавим нова система без да пипаме Hero.

---

## 7. Observer vs Event-Driven — сравнение

| Критерий              | Observer Pattern              | Event-Driven (EventBus)          |
|-----------------------|-------------------------------|----------------------------------|
| **Посредник**         | Subject държи списък с observers | EventBus е отделен компонент    |
| **Зависимост**        | Subject знае за Observer интерфейс | Publisher не знае за Subscriber |
| **Типове събития**    | Обикновено едно (промяна)     | Много типове с имена             |
| **Мащабиране**        | Умерено                       | Лесно — просто добавяш subscriber |
| **Debugging**         | По-лесно проследяване         | По-трудно — непряк поток        |
| **Подходящ за**       | Един обект с наблюдатели      | Много компоненти, различни събития |

---

### Кога да избереш кое?

```
Observer → когато един обект уведомява директни наблюдатели
           (ViewModel → View, Sensor → Display)

Event-Driven → когато много независими компоненти трябва
               да реагират на различни събития
               (GUI системи, игри, микросървиси)
```

---

## Резюме

```
Event-Driven Design = компонентите комуникират чрез събития

Publisher  →  публикува събитие  →  EventBus
                                        ↓
Subscriber ←  получава събитие  ←  EventBus

Предимства:
  ✓ Loose coupling — компонентите не знаят един за друг
  ✓ Лесно разширяване — нов subscriber без промяна на publisher
  ✓ Паралелна работа на екипа

Недостатъци:
  ✗ По-труден debugging — непряк поток на изпълнение
  ✗ Трябва дисциплина при именуване на събитията
```
