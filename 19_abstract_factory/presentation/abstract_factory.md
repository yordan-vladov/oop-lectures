# Abstract Factory

---

## Съдържание

* Какво е Abstract Factory
* Разлика между Factory и Abstract Factory
* Кога се използва
* Как се реализира Abstract Factory в C++

---

## Припомняне: Factory Pattern

Factory създава **един тип обект** чрез метод.

Пример:

```cpp
Shape* shape = ShapeFactory::createShape("circle");
```

Фабриката връща конкретен обект, но клиентът работи с абстракция.

---

## Какъв проблем остава?

Представете си GUI библиотека с различни стилове:

* Windows стил
* Linux стил
* Mac стил

---

Всеки стил има:

* Button
* CheckBox
* TextBox

---

Не искаме да смесваме Windows Button с Mac CheckBox.

Трябва да създаваме **свързани семейства от обекти**.

---

## Какво е Abstract Factory?

Abstract Factory е шаблон за проектиране, който предоставя интерфейс за създаване на **семейства от свързани обекти**, без да се указват техните конкретни класове.

Ключова разлика:

Factory → създава един продукт
Abstract Factory → създава група от продукти

---

## UML диаграма

![Image](https://www.uml-diagrams.org/examples/class-example-abstract-factory.png)

---

Обяснение:

* AbstractFactory - абстрактен клас
* ConcreteFactory1, ConcreteFactory2 - имплементират AbstractFactory
* AbstractProductA, AbstractProductB - Свързани с конкретни Factory класове
* ConcreteProductA1, ConcreteProductB1 - имплементират абстрактните класове

---

## Примерен сценарий: GUI система

Имаме:

Abstract продукти:

* Button
* CheckBox

---

Concrete продукти:

* WindowsButton
* MacButton
* WindowsCheckBox
* MacCheckBox

---

## Абстрактни продукти

```cpp
class Button {
public:
    virtual void render() = 0;
    virtual ~Button() = default;
};

class CheckBox {
public:
    virtual void render() = 0;
    virtual ~CheckBox() = default;
};
```

---

## Конкретни продукти (Windows)

```cpp
class WindowsButton : public Button {
public:
    void render() override {
        cout << "Render Windows Button\n";
    }
};

class WindowsCheckBox : public CheckBox {
public:
    void render() override {
        cout << "Render Windows CheckBox\n";
    }
};
```

---

## Конкретни продукти (Mac)

```cpp
class MacButton : public Button {
public:
    void render() override {
        cout << "Render Mac Button\n";
    }
};

class MacCheckBox : public CheckBox {
public:
    void render() override {
        cout << "Render Mac CheckBox\n";
    }
};
```

---

## Abstract Factory интерфейс

```cpp
class GUIFactory {
public:
    virtual Button* createButton() = 0;
    virtual CheckBox* createCheckBox() = 0;
    virtual ~GUIFactory() = default;
};
```

---

## Конкретни фабрики

```cpp
class WindowsFactory : public GUIFactory {
public:
    Button* createButton() override {
        return new WindowsButton();
    }

    CheckBox* createCheckBox() override {
        return new WindowsCheckBox();
    }
};

class MacFactory : public GUIFactory {
public:
    Button* createButton() override {
        return new MacButton();
    }

    CheckBox* createCheckBox() override {
        return new MacCheckBox();
    }
};
```

---

## Клиентски код

```cpp
void renderUI(GUIFactory& factory) {
    Button* btn = factory.createButton();
    CheckBox* cb = factory.createCheckBox();

    btn->render();
    cb->render();

    delete btn;
    delete cb;
}

int main() {
    WindowsFactory windowsFactory;
    renderUI(windowsFactory);
}
```

Клиентът не знае дали използва Windows или Mac.

---

## Подобрена версия (Modern C++)

```cpp
class GUIFactory {
public:
    virtual unique_ptr<Button> createButton() = 0;
    virtual unique_ptr<CheckBox> createCheckBox() = 0;
    virtual ~GUIFactory() = default;
};
```

Така избягваме ръчното управление на паметта.

---

## Предимства

* Гарантира съвместимост между продуктите
* Подпомага Open/Closed
* Лесно добавяне на ново семейство (напр. LinuxFactory)
* Централизира конфигурацията

---

## Недостатъци

* Трудно добавяне на нов тип продукт (напр. Slider)
* Повече класове
* По-сложна структура

---

## Factory vs Abstract Factory

| Factory              | Abstract Factory              |
| -------------------- | ----------------------------- |
| Създава един продукт | Създава семейство от продукти |
| Една фабрика         | Множество фабрики             |
| По-прост             | По-сложен                     |
| Фокус върху тип      | Фокус върху съвместимост      |

---

## Практическа задача

Създайте Abstract Factory за тема на приложение:

Семейства:

* LightThemeFactory
* DarkThemeFactory

---

Продукти:

* Button
* Menu
* Dialog

---

Изисквания:

* Абстрактни класове за трите продукта
* Абстрактна фабрика
* Две конкретни фабрики
* Клиентски код, който може да сменя темата без промяна в логиката

---

## Дискусия

* Кога бихме използвали Abstract Factory вместо Factory?
* Какво се случва ако добавим нов продукт към семейството?
* Кое е по-гъвкаво при добавяне на нов стил?
