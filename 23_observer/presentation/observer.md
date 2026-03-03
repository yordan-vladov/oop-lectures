# Observer

---

## Съдържание

* Какво е Observer pattern
* Какви проблеми решава този шаблон
* Принципът Loose Coupling (слаба зависимост)
* Как се реализира Observer в C++
* Modern C++ реализация

---

## Припомняне: Директна комуникация между обекти

Обикновено:

```cpp
class Display {
public:
    void update(int value) {
        std::cout << "Display: " << value << std::endl;
    }
};

class Sensor {
    Display* display;
public:
    Sensor(Display* d) : display(d) {}

    void setValue(int v) {
        display->update(v);
    }
};
```

---

* **Проблем:**

  * Sensor знае конкретния клас Display
  * Силна зависимост (tight coupling)
  * Трудно добавяне на нови наблюдатели

---

## Какъв проблем решава Observer?

Имаме система, в която:

* Един обект променя състоянието си (Subject)
* Много обекти трябва да бъдат уведомени (Observers)

Примери:

* GUI събития
* Stock market приложения
* Game engine събития
* Notification системи

---

## Основна идея

Вместо обектите да комуникират директно:

-  Subject поддържа списък с наблюдатели
- При промяна — уведомява всички

---

## Какво е Observer Pattern?

Observer е шаблон за проектиране, при който:

* Един обект (Subject) уведомява автоматично множество други обекти (Observers) при промяна на състоянието.

---

## UML структура (концепция)

* Subject — интерфейс за управление на наблюдатели
* Observer — интерфейс за получаване на обновления
* ConcreteSubject — реален обект със състояние
* ConcreteObserver — реални наблюдатели

---

## Пример: Интерфейси

### Observer

```cpp
class Observer {
public:
    virtual void update(int value) = 0;
    virtual ~Observer() {}
};
```

---

### Subject

```cpp
#include <vector>

class Subject {
protected:
    std::vector<Observer*> observers;

public:
    void attach(Observer* obs) {
        observers.push_back(obs);
    }

    void notify(int value) {
        for (auto obs : observers)
            obs->update(value);
    }

    virtual ~Subject() {}
};
```

---

## ConcreteSubject

```cpp
class Sensor : public Subject {
    int value = 0;

public:
    void setValue(int v) {
        value = v;
        notify(value);
    }
};
```

---

## ConcreteObservers

```cpp
class Display : public Observer {
public:
    void update(int value) override {
        std::cout << "Display: " << value << std::endl;
    }
};

class Logger : public Observer {
public:
    void update(int value) override {
        std::cout << "Logger записва: " << value << std::endl;
    }
};
```

---

## Използване

```cpp
int main() {
    Sensor sensor;

    Display display;
    Logger logger;

    sensor.attach(&display);
    sensor.attach(&logger);

    sensor.setValue(42);
}
```

---

## Какво постигнахме?

* Sensor не знае конкретните класове
* Можем да добавяме нови Observers без промяна
* Поддържаме слаба зависимост (loose coupling)

---

## Принцип: Loose Coupling

### Дефиниция

Обектите трябва да зависят от абстракции, а не от конкретни реализации.

---

### Observer помага защото:

* Subject познава само интерфейса Observer
* Няма нужда от if/else
* Нови класове се добавят лесно

---

## Подобрение: detach (отписване)

```cpp
void detach(Observer* obs) {
    observers.erase(
        std::remove(observers.begin(), observers.end(), obs),
        observers.end()
    );
}
```

---

## Modern C++ версия (smart pointers)

```cpp
#include <memory>
#include <vector>

class Observer {
public:
    virtual void update(int value) = 0;
    virtual ~Observer() = default;
};

class Subject {
    std::vector<std::weak_ptr<Observer>> observers;

public:
    void attach(std::shared_ptr<Observer> obs) {
        observers.push_back(obs);
    }

    void notify(int value) {
        for (auto& w : observers) {
            if (auto obs = w.lock())
                obs->update(value);
        }
    }
};
```

---

## Пример с shared_ptr

```cpp
class Display : public Observer {
public:
    void update(int value) override {
        std::cout << "Display: " << value << std::endl;
    }
};

int main() {
    Subject subject;

    auto display = std::make_shared<Display>();
    subject.attach(display);

    subject.notify(100);
}
```

---

## Проблем: Кой кога уведомява?

Има два варианта:

### Push модел

Subject изпраща данните:

```cpp
update(value);
```

### Pull модел

Observer взима данните:

```cpp
update();
observer->getData();
```

---

## Сравнение

| Push                        | Pull                           |
| --------------------------- | ------------------------------ |
| По-прост                    | По-гъвкав                      |
| Повече данни се изпращат    | Observer решава какво да вземе |
| По-малко заявки към Subject | Повече контрол                 |

---

## Кога да използваме Observer?

* Event системи
* GUI framework-и
* MVC архитектура
* Notification системи
* Игрови събития
* Reactive програмиране

---

## Предимства

* Слаба зависимост между обекти
* Лесно добавяне на нови наблюдатели
* Поддържа Open/Closed
* Подходящ за event-driven системи

---

## Недостатъци

* Трудно проследяване на събития
* Потенциални memory проблеми
* Възможни циклични зависимости
* Много уведомления при големи системи

---

## Практическа задача

Създайте система за новини:

* Subject: `NewsAgency`
* Observers:

  * `TVChannel`
  * `Radio`
  * `MobileApp`

---

### Изисквания

* Абстрактен клас `Observer` с метод `update(string news)`
* Клас `NewsAgency`, който пази списък с observers
* Метод `publishNews(string)`
* Демонстрация в main()
* Да се използва `smart pointers`

---

### Допълнително предизвикателство

Добавете нов Observer `Website`, без да променяте съществуващия код.

---

## Дискусия

* Кога е подходящо да използваме Observer?
* Как Observer намалява зависимостите?
* Какво се случва при много наблюдатели?
* Push или Pull модел е по-добър?

---

## Обобщение

* Observer позволява един обект да уведомява много други.
* Намалява зависимостите чрез интерфейси.
* Подходящ за event-driven архитектури.
* Често използван в реални системи и framework-и.
