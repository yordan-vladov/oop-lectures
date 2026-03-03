# Dependency Injection (DI)

---

## Съдържание

- Какво е зависимост в ООП
- Какво е силна свързаност (Tight Coupling) и SOLID
- Какво е Dependency Injection
- Видове Dependency Injection

---

## Какво е зависимост?

- В ООП една зависимост възниква, когато един клас използва друг клас, за да функционира.

---

```cpp
class Engine {
public:
    void start() {
        std::cout << "Engine started\n";
    }
};

class Car {
private:
    Engine engine;  
public:
    void drive() {
        engine.start();
    }
};
```

`Car` зависи директно от `Engine`.

---

## Какво е Tight Coupling?

- Tight Coupling означава силна обвързаност между класове.
- Характеристики:
    * Класът създава сам своите зависимости
    * Трудна подмяна на компоненти
    * Трудно тестване
- Проблем: Ако искаме ElectricEngine, трябва да променим класа Car.

---

## Защо това е проблем?

* Ниска гъвкавост
* Нарушава добрите ООП практики
* Затруднява unit testing
* Води до трудна поддръжка

---

## Какво е Dependency Injection?

- Техника, при която обектът не създава сам своите зависимости, а ги получава отвън.
- Цел:
    * Намаляване на свързаността
    * Работа с интерфейси 
    * По-гъвкава архитектура

---

## Видове Dependency Injection

- **Constructor Injection:** Зависимостта се подава чрез конструктора. Най-често използван.
- **Setter Injection:** Зависимостта се подава чрез setter метод.
- **Interface Injection:** По-рядко използван подход.
  
---

## Създаване на абстракция

```cpp
class IEngine {
public:
    virtual void start() = 0;
    virtual ~IEngine() {}
};
```

- Работим с интерфейс, а не с конкретен клас.

---

## Конкретни реализации

```cpp
class PetrolEngine : public IEngine {
public:
    void start() override {
        std::cout << "Petrol engine started\n";
    }
};

class ElectricEngine : public IEngine {
public:
    void start() override {
        std::cout << "Electric engine started\n";
    }
};
```

---

## Constructor Injection

```cpp
class Car {
private:
    IEngine* engine;

public:
    Car(IEngine* eng) : engine(eng) {}

    void drive() {
        engine->start();
    }
};
```

- Car не създава Engine. Получава го отвън.

---

## Използване

```cpp
int main() {
    PetrolEngine petrol;
    Car car(&petrol);
    car.drive();
}
```

---

- Смяна на зависимост:

```cpp
ElectricEngine electric;
Car ecoCar(&electric);
ecoCar.drive();
```

- Без промяна в класа Car.

---

## Подобрение със Smart Pointers

```cpp
#include <memory>

class Car {
private:
    std::shared_ptr<IEngine> engine;

public:
    Car(std::shared_ptr<IEngine> eng) : engine(eng) {}
};
```

---

- **Предимства:**
    * Безопасно управление на паметта
    * Избягване на memory leaks

---

## Предимства на Dependency Injection

* По-ниска свързаност
* По-лесно тестване
* Спазване на SOLID
* По-добра разширяемост
* По-добра поддръжка

---

## Недостатъци

* Повече абстракции
* Повече код
* Може да усложни малки проекти

---

## Практическа задача

1. Създайте интерфейс ILogger.
2. Реализирайте ConsoleLogger и FileLogger.
3. Създайте клас Application, който получава ILogger чрез constructor injection.
4. Демонстрирайте смяна на логера без промяна в Application.

---

## Обобщение

- Dependency Injection означава:
    * Не създавай зависимостите вътре в класа
    * Получавай ги отвън
    * Работи с абстракции
    * Следвай Dependency Inversion Principle
