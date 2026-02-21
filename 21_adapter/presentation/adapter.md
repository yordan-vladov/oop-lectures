# Adapter

---

## Съдържание

* Какво е Adapter pattern
* Какви проблеми решава този шаблон
* Какво е несъвместим интерфейс
* Как се реализира Adapter в C++

---

## Припомняне: Работа с интерфейси в C++

Обикновено използваме класове с общ интерфейс:

```cpp
class Printer {
public:
    virtual void print(string text) = 0;
};
```

Клиентът очаква точно този метод `print()`.

---

* Проблем възниква когато имаме съществуващ клас с различен интерфейс:

```cpp
class LegacyPrinter {
public:
    void printText(string text) {
        cout << text;
    }
};
```

---

* **Проблем:**

  * Интерфейсите не съвпадат
  * Не можем да използваме класа директно
  * Не искаме да променяме съществуващия код

---

## Какъв проблем решава Adapter?

Имаме система, която работи с:

* AudioPlayer
* VideoPlayer
* ImageViewer

Но получаваме нов клас:

* ThirdPartyMediaPlayer

който има различни методи.

---

Без Adapter трябва да променим клиентския код.

---

## Какво е несъвместим интерфейс?

Несъвместим интерфейс означава:

* Методите имат различни имена
* Различни параметри
* Различна структура

---

Пример:

Очакван интерфейс:

```cpp
play("song.mp3");
```

Реален клас:

```cpp
startPlayback("song.mp3");
```

---

## Какво е Adapter Pattern?

Adapter е шаблон за проектиране, който:

* Преобразува интерфейса на един клас в интерфейс, който клиентът очаква
* Позволява работа между несъвместими класове
* Използва обвивка (wrapper) около съществуващ обект

---

Основна идея:

> Adapter = преводач между два интерфейса

---

## Структура

* Target – интерфейсът, който клиентът очаква
* Adaptee – съществуващият клас
* Adapter – класът преводач
* Client – кодът, който използва Target

---

## Пример: Media Player

### Target интерфейс

```cpp
class MediaPlayer {
public:
    virtual void play(const std::string& filename) = 0;
    virtual ~MediaPlayer() {}
};
```

---

### Adaptee (несъвместим клас)

```cpp
class AdvancedPlayer {
public:
    void startPlayback(const std::string& file) {
        std::cout << "Playing: " << file << std::endl;
    }
};
```

---

### Adapter клас

```cpp
class PlayerAdapter : public MediaPlayer {
private:
    AdvancedPlayer advancedPlayer;

public:
    void play(const std::string& filename) override {
        advancedPlayer.startPlayback(filename);
    }
};
```

---

### Използване

```cpp
int main() {
    MediaPlayer* player = new PlayerAdapter();
    player->play("song.mp3");

    delete player;
}
```

---

## Какво се случва?

Клиентът извиква:

```
player->play()
```

Adapter превежда към:

```
advancedPlayer.startPlayback()
```

---

## Подобрена версия (Modern C++)

```cpp
#include <memory>

int main() {
    std::unique_ptr<MediaPlayer> player =
        std::make_unique<PlayerAdapter>();

    player->play("song.mp3");
}
```

---

## Adapter с подаване на обект (Object Adapter)

По-гъвкав вариант — подаваме adaptee отвън.

```cpp
class PlayerAdapter : public MediaPlayer {
private:
    AdvancedPlayer* advancedPlayer;

public:
    PlayerAdapter(AdvancedPlayer* player)
        : advancedPlayer(player) {}

    void play(const std::string& filename) override {
        advancedPlayer->startPlayback(filename);
    }
};
```

---

## Предимство

* Можем да адаптираме различни обекти динамично
* Не създаваме adaptee вътре

---

## Реален пример от практиката

* Работа със стари библиотеки
* Интеграция на външни API
* Legacy код
* Хардуерни драйвери

---

## Пример: Зарядно за телефон

* Контактът е 220V
* Телефонът работи на 5V
* Зарядното е Adapter

---

## Adapter и Open/Closed принцип

Adapter подпомага OCP, защото:

* Не променяме съществуващите класове
* Добавяме нов Adapter клас
* Разширяваме поведението без модификация

---

## Class Adapter vs Object Adapter

| Class Adapter      | Object Adapter     |
| ------------------ | ------------------ |
| Наследяване        | Композиция         |
| По-рядко използван | По-често използван |
| Ограничен от езика | По-гъвкав          |

---

## Кога да използваме Adapter?

* Когато интерфейсите не съвпадат
* Когато използваме чужда библиотека
* Когато имаме legacy код
* Когато не можем да променим съществуващ клас

---

## Обобщение

* Adapter свързва несъвместими интерфейси
* Позволява повторна употреба на съществуващ код
* Работи като преводач между класове
* Подобрява гъвкавостта на системата

---

## Предимства

* Повторна употреба на код
* Спазване на Open/Closed
* Изолиране на промени
* По-добра архитектура

---

## Недостатъци

* Допълнителен слой абстракция
* Повече класове
* Може да усложни дизайна

---

## Практическа задача

Създайте Adapter за система за плащане.

Системата използва интерфейс:

* `PaymentProcessor` с метод `pay(double amount)`

Имаме външни класове:

* `PayPalAPI` с метод `sendPayment(double amount)`
* `StripeAPI` с метод `makeCharge(double amount)`

---

### Изисквания

* Абстрактен клас `PaymentProcessor`
* Adapter класове:

  * `PayPalAdapter`
  * `StripeAdapter`
* Демонстрация в main()
* Да се използва `unique_ptr`

---

### Допълнително предизвикателство

Добавете нова платежна система `CryptoAPI`, без да променяте клиентския код.

---

## Дискусия

* Кога Adapter е по-подходящ от Factory?
* Как Adapter подпомага повторната употреба?
* Какви проблеми решава при интеграция на библиотеки?
