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

**Проблем:**

* Клиентският код знае конкретния клас
* Силна зависимост (tight coupling)
* Трудно разширение

---

## Какъв проблем решава Factory?

Имаме система за създаване на различни елементи:

* Car
* Truck
* Motorcycle

---

Ако използваме много if/else:

```cpp
if(type == "car") return new Car();
else if(type == "truck") return new Truck();
```

При добавяне на нов тип трябва да променяме съществуващ код.

---

## Принцип Open/Closed (OCP)

### Дефиниция

Софтуерните компоненти трябва да бъдат:

* Отворени за разширение
* Затворени за модификация

---

Това означава:

Можем да добавяме ново поведение чрез нови класове,
без да променяме вече работещия код.

---

### Пример за нарушение на OCP

```cpp
if(type == "circle") ...
else if(type == "square") ...
else if(type == "triangle") ...
```

Всеки нов тип изисква редакция на съществуващ код.

---

### Как Factory помага?

* Клиентският код работи с абстракция (интерфейс)
* Новите типове се добавят чрез нов клас
* Промените са локализирани

Така системата е по-стабилна и разширяема.

---

## Какво е Factory Pattern?

Factory е шаблон за проектиране, който делегира създаването на обекти към специален клас (фабрика).

Обектите се създават чрез метод, а не директно с `new`.

---

![Image](https://www.tutorialspoint.com/design_pattern/images/factory_pattern_uml_diagram.jpg)

---

* Shape – абстрактен клас (интерфейс)
* Circle, Square, Rectangle – конкретни класове
* ShapeFactory – създава обекти

---

## Пример: Shape Factory

Абстрактен клас:

```cpp
class Shape {
public:
    virtual void draw() = 0;
    virtual ~Shape() {}
};
```

---

Конкретни класове:

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

Factory клас:

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

Използване:

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

Създайте Factory за GUI библиотека.

Системата трябва да създава различни графични компоненти:

* Button
* TextBox
* CheckBox

---

Изисквания:

* Абстрактен клас `UIElement` с виртуален метод `render()`
* Класове `Button`, `TextBox`, `CheckBox`, които наследяват `UIElement`
* Клас `UIFactory` със static метод `createElement(string type)`
* Демонстрация в main()
* Да се използва `unique_ptr`

---

Допълнително предизвикателство:

Добавете нов компонент `Slider`, без да променяте клиентския код в main().

---

## Дискусия

* Кога е подходящо да използваме Factory?
* Как Factory подпомага спазването на OCP?
* Какво се случва при добавяне на нов тип?
