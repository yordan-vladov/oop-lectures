# Builder Design Pattern

---

## Въведение

- При разработване на софтуер често се налага създаване на сложни обекти с множество параметри и различни конфигурации.
- Пример за проблем:

```cpp
Car car("V8", true, true, false, "Red", 4, ...);
```

---

- Кодът е труден за четене, лесно се допускат грешки и поддръжката става сложна.
- Необходимо е решение, което позволява поетапно и контролирано създаване на обекти.

---

## Какво представлява Builder Pattern

- Builder е шаблон за проектиране, който позволява създаването на сложен обект стъпка по стъпка.
- Същият процес на изграждане може да създава различни представяния на обекта.
- Основната идея е да се отдели процесът на конструиране от крайния обект.

---

## UML структура на Builder

![Image](https://www.patrickschadler.com/content/images/2020/10/Builder-Pattern-UML.png)

---

Основни участници:

* Product – крайният обект
* Builder – интерфейс с методи за изграждане
* ConcreteBuilder – конкретна реализация
* Director – управлява процеса по изграждане

---

## Пример от реалния живот

- Фирма строи компютри и лаптопи.
- Процесът включва:
    * процесор
    * RAM
    * Диск
    * Графична карта
- Една и съща последователност може да създаде различни видове сгради – настолен компютър, гейминг компютър и др.

---

## Пример

---

### Product – Computer

```cpp
#include <iostream>
#include <string>

class Computer {
public:
    std::string CPU;
    std::string RAM;
    std::string Storage;
    bool hasGPU;

    void showSpecs() {
        std::cout << "CPU: " << CPU << std::endl;
        std::cout << "RAM: " << RAM << std::endl;
        std::cout << "Storage: " << Storage << std::endl;
        std::cout << "GPU: " << (hasGPU ? "Yes" : "No") << std::endl;
    }
};
```

---
### Builder интерфейс

```cpp
class ComputerBuilder {
public:
    virtual void buildCPU() = 0;
    virtual void buildRAM() = 0;
    virtual void buildStorage() = 0;
    virtual void buildGPU() = 0;
    virtual Computer* getComputer() = 0;
    virtual ~ComputerBuilder() {}
};
```

---
### Concrete Builder

```cpp
class GamingComputerBuilder : public ComputerBuilder {
private:
    Computer* computer;

public:
    GamingComputerBuilder() {
        computer = new Computer();
    }

    void buildCPU() override {
        computer->CPU = "Intel i9";
    }

    void buildRAM() override {
        computer->RAM = "32GB";
    }

    void buildStorage() override {
        computer->Storage = "1TB SSD";
    }

    void buildGPU() override {
        computer->hasGPU = true;
    }

    Computer* getComputer() override {
        return computer;
    }
};
```

---

### Director

```cpp
class Director {
public:
    void construct(ComputerBuilder* builder) {
        builder->buildCPU();
        builder->buildRAM();
        builder->buildStorage();
        builder->buildGPU();
    }
};
```

---

### Използване

```cpp
int main() {
    Director director;
    GamingComputerBuilder builder;

    director.construct(&builder);
    Computer* gamingPC = builder.getComputer();

    gamingPC->showSpecs();

    delete gamingPC;
    return 0;
}
```

---

## Модерен Builder (Fluent Interface)

В практиката често се използва вариант без Director, при който методите връщат референция към текущия обект.

```cpp
class ComputerBuilder {
private:
    Computer computer;

public:
    ComputerBuilder& setCPU(std::string cpu) {
        computer.CPU = cpu;
        return *this;
    }

    ComputerBuilder& setRAM(std::string ram) {
        computer.RAM = ram;
        return *this;
    }

    ComputerBuilder& setStorage(std::string storage) {
        computer.Storage = storage;
        return *this;
    }

    Computer build() {
        return computer;
    }
};
```

---

Използване:

```cpp
Computer pc = ComputerBuilder()
                .setCPU("Ryzen 7")
                .setRAM("16GB")
                .setStorage("512GB SSD")
                .build();
```

---

## Кога използваме Builder

* Когато обектът има много параметри
* Когато има различни конфигурации
* Когато искаме по-четим и безопасен код
* Когато искаме да избегнем множество претоварени конструктори

---

## Сравнение с други шаблони

Factory – създава обекта наведнъж.
Abstract Factory – създава семейство свързани обекти.
Builder – създава един сложен обект стъпка по стъпка.

---

## Предимства и недостатъци

- Предимства:
    * По-ясна структура
    * По-добра четимост
    * Гъвкавост при конфигуриране

---

- Недостатъци:
    * Повече класове
    * По-сложна архитектура

---

## Практическа задача 

---

### Контекст на приложението

- Разработвате модул за система за управление на събития (Event Management System).
- Системата позволява създаване на различни типове събития с множество параметри и специфични изисквания.

---

- Типове събития:
    * Онлайн уебинар
    * Присъствена конференция
    * Хибридно събитие

---

### Основни характеристики на събитие

- Всеки обект `Event` може да съдържа:
    * Заглавие
    * Дата и продължителност
    * Лектори
    * Максимален брой участници

---

- Обектът също така може да съдържа:
    * Тип достъп (безплатен / платен)
    * Цена (ако е платено)
    * Локация (за присъствени)
    * Онлайн линк (за онлайн)
- Обектът не трябва да може да съществува в невалидно състояние.

---

### Бизнес правила

* Уебинарът задължително има онлайн линк.
* Конференцията задължително има локация.
* Хибридното събитие има и двете.
* Платено събитие трябва да има валидна цена.

---

### Изисквания към реализацията

1. Създайте клас `Event` (Product).
2. Дефинирайте абстрактен `EventBuilder`.
3. Реализирайте:
   * `WebinarBuilder`
   * `ConferenceBuilder`
   * `HybridEventBuilder`

---

4. Създайте `EventDirector` за стандартни конфигурации.
5. Демонстрирайте създаване на различни събития в `main()`.

---

### Допълнително

* Реализирайте Fluent Builder за персонализирано събитие.
* Добавете метод за изчисляване на крайна цена.

---

- Целта е да се гарантира контролирано и валидно изграждане на сложен обект чрез Builder pattern.

---

## Обобщение

- Builder Pattern позволява поетапно и контролирано създаване на сложни обекти.
- Разделя процеса на конструиране от крайния резултат.
- Използва се често в реални приложения, особено при конфигурируеми продукти, API библиотеки и сложни структури от данни.
