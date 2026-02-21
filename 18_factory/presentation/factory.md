# Factory

---

## Съдържание

* Kакво е Factory pattern
* Какви проблеми решава този шаблон
* Какво е принципът Open/Closed
* Как се реализира Factory в C++

---

## Припомняне: Създаване на обекти в C++

Обикновено:

```cpp
Dog* dog = new Dog();
Cat* cat = new Cat();
```

---

- **Проблем:**
    * Клиентският код знае конкретния клас
    * Силна зависимост (tight coupling)
    * Трудно разширение

---

## Какъв проблем решава Factory?

- Имаме система за създаване на различни елементи:
    * Car
    * Truck
    * Motorcycle

---

- Ако използваме много if/else:

```cpp
if(type == "car") return new Car();
else if(type == "truck") return new Truck();
```

- При добавяне на нов тип трябва да променяме съществуващ код.

---

## Принцип Open/Closed (OCP)

### Дефиниция

- Софтуерните компоненти трябва да бъдат:
    * Отворени за разширение
    * Затворени за модификация

---

- Това означава:
    - Можем да добавяме ново поведение чрез нови класове, без да променяме вече работещия код.

---

### Пример за нарушение на OCP

```cpp
if(type == "circle") ...
else if(type == "square") ...
else if(type == "triangle") ...
```

- Всеки нов тип изисква редакция на съществуващ код.

---

### Как Factory помага?

* Клиентският код работи с абстракция (интерфейс)
* Новите типове се добавят чрез нов клас
* Промените са локализирани

- Така системата е по-стабилна и разширяема.

---

## Какво е Factory Pattern?

- Factory е шаблон за проектиране, който делегира създаването на обекти към специален клас (фабрика).

- Обектите се създават чрез метод, а не директно с `new`.

---

![Image](https://www.tutorialspoint.com/design_pattern/images/factory_pattern_uml_diagram.jpg)

---

* Shape – абстрактен клас (интерфейс)
* Circle, Square, Rectangle – конкретни класове
* ShapeFactory – създава обекти

---

## Пример: Shape Factory

- Абстрактен клас:

```cpp
class Shape {
public:
    virtual void draw() = 0;
    virtual ~Shape() {}
};
```

---

- Конкретни класове:

```cpp
class Circle : public Shape {
public:
    void draw() override {
        cout << "Drawing Circle\n";
    }
};

class Square : public Shape {
public:
    void draw() override {
        cout << "Drawing Square\n";
    }
};
```

---

- Factory клас:

```cpp
class ShapeFactory {
public:
    static Shape* createShape(string type) {
        if (type == "circle")
            return new Circle();
        else if (type == "square")
            return new Square();
        else
            return nullptr;
    }
};
```

---

- Използване:

```cpp
int main() {
    Shape* shape1 = ShapeFactory::createShape("circle");
    Shape* shape2 = ShapeFactory::createShape("square");

    shape1->draw();
    shape2->draw();

    delete shape1;
    delete shape2;
}
```

---

## Подобрена версия (Modern C++)

```cpp
#include <memory>

class ShapeFactory {
public:
    static unique_ptr<Shape> createShape(string type) {
        if (type == "circle")
            return make_unique<Circle>();
        else if (type == "square")
            return make_unique<Square>();
        return nullptr;
    }
};
```


---

## Проблем с текущата реализация

```cpp
static unique_ptr<Shape> createShape(string type) {
    if (type == "circle")
        return make_unique<Circle>();
    else if (type == "square")
        return make_unique<Square>();
    return nullptr;
}
```

* При добавяне на нов клас трябва да променим фабриката.
* Това нарушава принципа Open/Closed.
* Фабриката расте с всеки нов тип.

---

## Решение: Factory с регистър (Registry-based Factory)

Идея:

* Фабриката не знае конкретните класове.
* Всеки клас сам се регистрира във фабриката.
* При добавяне на нов тип не се променя фабриката.

---

## Реализация: ShapeFactory с регистър

```cpp
#include <memory>
#include <unordered_map>
#include <functional>
#include <string>

class Shape;

class ShapeFactory {
public:
    using Creator = std::function<std::unique_ptr<Shape>()>;

    static bool registerType(const std::string& type, Creator creator) {
        registry()[type] = creator;
        return true;
    }

    static std::unique_ptr<Shape> createShape(const std::string& type) {
        auto it = registry().find(type);
        if (it != registry().end())
            return it->second();
        return nullptr;
    }

private:
    static std::unordered_map<std::string, Creator>& registry() {
        static std::unordered_map<std::string, Creator> instance;
        return instance;
    }
};
```

---

## Саморегистрация на Circle

```cpp
class Circle : public Shape {
public:
    void draw() override {
        std::cout << "Drawing Circle\n";
    }
};

namespace {
    const bool registered =
        ShapeFactory::registerType("circle", [] {
            return std::make_unique<Circle>();
        });
}
```

---

## Саморегистрация на Square

```cpp
class Square : public Shape {
public:
    void draw() override {
        std::cout << "Drawing Square\n";
    }
};

namespace {
    const bool registered =
        ShapeFactory::registerType("square", [] {
            return std::make_unique<Square>();
        });
}
```

---

## Добавяне на нов тип (Triangle)

```cpp
class Triangle : public Shape {
public:
    void draw() override {
        std::cout << "Drawing Triangle\n";
    }
};

namespace {
    const bool registered =
        ShapeFactory::registerType("triangle", [] {
            return std::make_unique<Triangle>();
        });
}
```

* Не променяме фабриката.
* Не променяме main().
* Добавяме само нов клас.

---

## Защо това спазва OCP?

* Фабриката не се модифицира.
* Новото поведение се добавя чрез нов клас.
* Системата е отворена за разширение, затворена за модификация.

---

## Сравнение

| Simple Factory             | Factory с регистър              |
| -------------------------- | ------------------------------- |
| Използва if/else           | Използва map от функции         |
| Нарушава OCP               | Спазва OCP                      |
| По-лесна за разбиране      | По-гъвкава                      |
| Подходяща за малки системи | Подходяща за разширяеми системи |

---

## Кога да използваме регистър?

* Plugin системи
* Game engines
* Системи с динамично добавяне на типове
* Framework-и

---

## Обобщение

* Simple Factory централизира създаването, но не гарантира OCP.
* Registry-based Factory премахва нуждата от if/else.
* Добавянето на нов тип не изисква промяна в съществуващ код.
* Това е по-близко до реални индустриални реализации.

---

## Предимства

* Намалява зависимостите
* Подпомага спазването на Open/Closed
* Централизира създаването на обекти
* Улеснява разширението

---

## Недостатъци

* Повече класове
* Допълнителна абстракция
* При малки проекти може да е излишно

---

## Практическа задача

- Създайте Factory за GUI библиотека.
- Системата трябва да създава различни графични компоненти:
    * Button
    * TextBox
    * CheckBox

---

- Изисквания:
    * Абстрактен клас `UIElement` с виртуален метод `render()`
    * Класове `Button`, `TextBox`, `CheckBox`, които наследяват `UIElement`
    * Клас `UIFactory` със static метод `createElement(string type)`
    * Демонстрация в main()
    * Да се използва `unique_ptr`

---

- Допълнително предизвикателство:
    - Добавете нов компонент `Slider`, без да променяте клиентския код в main().

---

## Дискусия

* Кога е подходящо да използваме Factory?
* Как Factory подпомага спазването на OCP?
* Какво се случва при добавяне на нов тип?
