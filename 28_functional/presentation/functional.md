# Функционално Програмиране в C++

---

## Съдържание

1. Какво е Функционално Програмиране?
2. Чисти функции
3. Неизменяемост (Immutability)
4. Ламбда функции и затваряния
5. Функции от по-висок ред
6. `std::transform`, `std::copy_if`, `std::accumulate`
7. Функтори
8. Частично прилагане и `std::bind`
9. Композиция на функции
10. FP срещу OOP — кога кое?
11. Резюме

---

## 1. Какво е Функционално Програмиране?

**Функционалното програмиране (FP)** е парадигма, при която програмата се изгражда от **функции без странични ефекти**.

Основни принципи:
- **Чисти функции** — резултатът зависи само от входа
- **Неизменяемост** — данните не се променят, създават се нови
- **Функции като стойности** — функциите се предават и връщат като всяка друга стойност
- **Декларативен стил** — описваш *какво* да се направи, не *как*

> C++ не е функционален език, но поддържа FP стил чрез ламбди, `std::function` и алгоритмите от `<algorithm>`.

---

### Императивен срещу Функционален стил

```cpp
// Императивен (как): ръчен цикъл
std::vector<int> nums = {1, 2, 3, 4, 5};
std::vector<int> result;
for (int x : nums) {
    if (x % 2 == 0) {
        result.push_back(x * x);
    }
}

// Функционален (какво): описваш трансформацията
std::vector<int> evens;
std::copy_if(nums.begin(), nums.end(), std::back_inserter(evens),
    [](int x) { return x % 2 == 0; });

std::vector<int> squared(evens.size());
std::transform(evens.begin(), evens.end(), squared.begin(),
    [](int x) { return x * x; });
```

---

## 2. Чисти функции

**Чиста функция** = функция, която:
1. При едни и същи входове **винаги връща един и същ резултат**
2. **Няма странични ефекти** — не променя глобално състояние, не пише в конзолата, не модифицира аргументите

---

### Нечиста срещу Чиста функция

```cpp
// НЕЧИСТА — зависи от глобално състояние
int counter = 0;

int nextId() {
    return ++counter; // страничен ефект: модифицира counter
}

// Всяко извикване дава различен резултат!
nextId(); // → 1
nextId(); // → 2
```

```cpp
// ЧИСТА — зависи само от параметрите
int add(int a, int b) {
    return a + b; // без странични ефекти
}

// Винаги един и същ резултат за едни и същи входове
add(3, 4); // → 7
add(3, 4); // → 7 (гарантирано)
```

---

### Защо са важни чистите функции?

```
Предимства:
  ✓ Лесни за тестване — не е нужен mock на глобално състояние
  ✓ Лесни за разсъждение — резултатът е предвидим
  ✓ Безопасни за паралелно изпълнение — без race conditions
  ✓ Могат да се кешират (memoize)

Недостатъци:
  ✗ Не всичко може да е чисто (I/O, мрежа, UI)
```

> **Правило**: Дръж бизнес логиката в чисти функции. Изолирай страничните ефекти на границите на системата.

---

## 3. Неизменяемост (Immutability)

В FP данните **не се модифицират** — вместо промяна се създава **нова стойност**.

```cpp
// Мутиращ стил
std::vector<int> nums = {1, 2, 3};
nums.push_back(4); // nums е променен

// Неизменяем стил — работим с const и създаваме нов вектор
const std::vector<int> original = {1, 2, 3};

std::vector<int> extended = original; // копие
extended.push_back(4);                // original не е засегнат
```

---

### `const` корекстност

```cpp
// const параметри — функцията не може да промени входа
double area(const double width, const double height) {
    return width * height;
}

// const референции — ефективно предаване без копиране
std::string toUpper(const std::string& s) {
    std::string result = s; // работим с копие
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

int main() {
    std::string name = "alice";
    std::string upper = toUpper(name);
    // name е непроменен, upper е "ALICE"
}
```

---

## 4. Ламбда функции и затваряния

**Ламбда** = анонимна функция, дефинирана на място.

```
[capture](параметри) -> тип_връщане { тяло }
```

```cpp
// Проста ламбда
auto square = [](int x) { return x * x; };
std::cout << square(5); // → 25

// С явен тип на връщане
auto divide = [](double a, double b) -> double {
    return a / b;
};

// Предаване като аргумент
auto applyTwice = [](auto f, int x) { return f(f(x)); };
std::cout << applyTwice(square, 3); // → 81
```

---

### Capture — затваряния

Ламбдата може да **"затвори"** (capture) променливи от обкръжаващия обхват.

```cpp
int base = 10;

// [=] — capture по стойност (копие)
auto addBase = [=](int x) { return x + base; };
base = 99;
std::cout << addBase(5); // → 15 (използва base=10 от момента на дефиниция)

// [&] — capture по референция (живи данни)
auto addBaseLive = [&](int x) { return x + base; };
std::cout << addBaseLive(5); // → 104 (използва текущото base=99)

// Конкретна capture
auto mul = [base](int x) { return x * base; }; // само base по стойност
```

---

### Кога да използваш кой capture?

```
[=]  — когато ламбдата може да надживее локалните промени
       (например съхранена в вектор от callbacks)

[&]  — когато ламбдата се извиква веднага и данните са живи

[x]  — само конкретна променлива (препоръчително за яснота)
```

```cpp
// Опасно! base е destroyed преди callback-а да бъде извикан
auto makeCallback() {
    int base = 42;
    return [&]() { return base; }; // dangling reference!
}

// Безопасно — capture по стойност
auto makeCallback() {
    int base = 42;
    return [=]() { return base; }; // копие, безопасно
}
```

---

## 5. Функции от по-висок ред

**Функция от по-висок ред** = функция, която:
- **приема** функция като аргумент, или
- **връща** функция като резултат

```cpp
#include <functional>

// Приема функция като параметър
void applyToAll(const std::vector<int>& v, std::function<void(int)> f) {
    for (int x : v) f(x);
}

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5};

    applyToAll(nums, [](int x) {
        std::cout << x * x << " ";
    });
    // → 1 4 9 16 25
}
```

---

### Връщане на функция

```cpp
// Фабрика за функции — връща ламбда
auto makeMultiplier(int factor) {
    return [factor](int x) { return x * factor; };
}

auto double_it = makeMultiplier(2);
auto triple    = makeMultiplier(3);

std::cout << double_it(5); // → 10
std::cout << triple(5);    // → 15
```

```cpp
// Фабрика за валидатори
auto inRange(int min, int max) {
    return [min, max](int x) { return x >= min && x <= max; };
}

auto isAdult    = inRange(18, 120);
auto isValidAge = inRange(0, 150);

std::cout << isAdult(20);    // → 1 (true)
std::cout << isAdult(15);    // → 0 (false)
```

---

## 6. `std::transform`, `std::copy_if`, `std::accumulate`

Стандартните алгоритми от `<algorithm>` и `<numeric>` са основни инструменти за FP в C++.

---

### `std::transform` — map

Прилага функция върху всеки елемент.

```cpp
#include <algorithm>
#include <vector>

std::vector<int> nums = {1, 2, 3, 4, 5};
std::vector<int> squared(nums.size());

std::transform(nums.begin(), nums.end(), squared.begin(),
    [](int x) { return x * x; });
// squared = {1, 4, 9, 16, 25}

// Трансформация на стрингове
std::vector<std::string> words = {"hello", "world"};
std::vector<std::string> upper(words.size());

std::transform(words.begin(), words.end(), upper.begin(),
    [](std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    });
// upper = {"HELLO", "WORLD"}
```

---

### `std::copy_if` — filter

Копира само елементите, отговарящи на условие.

```cpp
std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
std::vector<int> evens;

std::copy_if(nums.begin(), nums.end(), std::back_inserter(evens),
    [](int x) { return x % 2 == 0; });
// evens = {2, 4, 6, 8, 10}

// Филтриране на стрингове по дължина
std::vector<std::string> words = {"cat", "elephant", "dog", "rhinoceros"};
std::vector<std::string> longWords;

std::copy_if(words.begin(), words.end(), std::back_inserter(longWords),
    [](const std::string& s) { return s.size() > 4; });
// longWords = {"elephant", "rhinoceros"}
```

---

### `std::accumulate` — reduce/fold

Свива колекция до единична стойност.

```cpp
#include <numeric>

std::vector<int> nums = {1, 2, 3, 4, 5};

// Сума
int sum = std::accumulate(nums.begin(), nums.end(), 0,
    [](int acc, int x) { return acc + x; });
// → 15

// Произведение
int product = std::accumulate(nums.begin(), nums.end(), 1,
    [](int acc, int x) { return acc * x; });
// → 120

// Конкатенация на стрингове
std::vector<std::string> words = {"C++", "е", "мощен"};
std::string sentence = std::accumulate(words.begin(), words.end(), std::string{},
    [](const std::string& acc, const std::string& word) {
        return acc.empty() ? word : acc + " " + word;
    });
// → "C++ е мощен"
```

---

### Пример: Map + Filter + Reduce верига

```cpp
// Намери сумата на квадратите на четните числа от 1 до 10
std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// 1. Filter — само четните
std::vector<int> evens;
std::copy_if(nums.begin(), nums.end(), std::back_inserter(evens),
    [](int x) { return x % 2 == 0; });
// evens = {2, 4, 6, 8, 10}

// 2. Map — квадрат на всеки
std::vector<int> squared(evens.size());
std::transform(evens.begin(), evens.end(), squared.begin(),
    [](int x) { return x * x; });
// squared = {4, 16, 36, 64, 100}

// 3. Reduce — сума
int total = std::accumulate(squared.begin(), squared.end(), 0);
// → 220

std::cout << total; // → 220
```

---

## 7. Функтори

**Функтор** = обект, чийто клас дефинира `operator()`. Ведне се използва като функция.

```cpp
// Ламбда (по-кратко)
auto add10 = [](int x) { return x + 10; };

// Функтор (по-гъвкаво — може да пази вътрешно състояние)
class Adder {
    int value;
public:
    explicit Adder(int v) : value(v) {}
    int operator()(int x) const { return x + value; }
};

Adder add5(5);
Adder add100(100);

std::cout << add5(3);   // → 8
std::cout << add100(3); // → 103

// Работи с std::transform
std::vector<int> nums = {1, 2, 3};
std::vector<int> result(nums.size());
std::transform(nums.begin(), nums.end(), result.begin(), Adder(10));
// result = {11, 12, 13}
```

---

### Функтори срещу Ламбди

| Критерий | Ламбда | Функтор (клас) |
|---|---|---|
| **Синтаксис** | Кратък, вграден | По-подробен |
| **Многократна употреба** | Трудна (анонимна) | Лесна (именован клас) |
| **Вътрешно състояние** | Чрез capture | Чрез член-данни |
| **Наследяване** | Не | Да |
| **Тестване** | Трудно отделно | Лесно |

> **Правило**: Използвай ламбди за прости, еднократни операции. Функтори — за по-сложна логика с много употреба.

---

## 8. Частично прилагане и `std::bind`

**Частично прилагане** = фиксираш някои аргументи на функция и получаваш нова функция с по-малко аргументи.

```cpp
#include <functional>

int multiply(int a, int b) {
    return a * b;
}

// std::bind — фиксира a=2, b остава свободен
auto double_it = std::bind(multiply, 2, std::placeholders::_1);
auto triple    = std::bind(multiply, 3, std::placeholders::_1);

std::cout << double_it(5);  // → 10
std::cout << triple(7);     // → 21

// Работи с std::transform
std::vector<int> nums = {1, 2, 3, 4, 5};
std::vector<int> doubled(nums.size());
std::transform(nums.begin(), nums.end(), doubled.begin(), double_it);
// doubled = {2, 4, 6, 8, 10}
```

---

### Частично прилагане с ламбди (по-модерен стил)

```cpp
// std::bind е малко остарял — ламбдите са по-четими
auto makeMultiplier(int factor) {
    return [factor](int x) { return x * factor; };
}

auto double_it = makeMultiplier(2);
auto triple    = makeMultiplier(3);

// Частично прилагане за std::sort
std::vector<std::string> words = {"banana", "apple", "cherry"};

auto byLength = [](const std::string& a, const std::string& b) {
    return a.size() < b.size();
};

std::sort(words.begin(), words.end(), byLength);
// words = {"apple", "banana", "cherry"}
```

---

## 9. Композиция на функции

**Функционална композиция** = изграждане на сложна функция от прости.

```
compose(f, g)(x) = f(g(x))
```

```cpp
// Ръчна композиция с ламбди
auto compose = [](auto f, auto g) {
    return [f, g](auto x) { return f(g(x)); };
};

auto addOne    = [](int x) { return x + 1; };
auto double_it = [](int x) { return x * 2; };

auto addOneThenDouble = compose(double_it, addOne); // double_it(addOne(x))
auto doubleThenAddOne = compose(addOne, double_it); // addOne(double_it(x))

std::cout << addOneThenDouble(3); // → (3+1)*2 = 8
std::cout << doubleThenAddOne(3); // → 3*2+1 = 7
```

---

### Тръбопровод (Pipeline)

```cpp
// Верига от трансформации
auto pipeline = [](auto value, auto... funcs) {
    return (... , (value = funcs(value)), value);
    // C++17 fold expression
};

// По-четимо с последователни извиквания
auto process(int x) {
    auto addOne    = [](int v) { return v + 1; };
    auto double_it = [](int v) { return v * 2; };
    auto square    = [](int v) { return v * v; };

    return square(double_it(addOne(x)));
}

std::cout << process(3); // → ((3+1)*2)^2 = 64
```

---

### Реален пример: обработка на данни

```cpp
struct Student {
    std::string name;
    int grade;
};

std::vector<Student> students = {
    {"Алис",  85},
    {"Боби",  42},
    {"Карла", 91},
    {"Дими",  58},
    {"Ева",   76},
};

// 1. Филтрирай успешните (grade >= 60)
std::vector<Student> passed;
std::copy_if(students.begin(), students.end(), std::back_inserter(passed),
    [](const Student& s) { return s.grade >= 60; });

// 2. Извлечи само оценките
std::vector<int> grades(passed.size());
std::transform(passed.begin(), passed.end(), grades.begin(),
    [](const Student& s) { return s.grade; });

// 3. Намери средната оценка
double avg = static_cast<double>(
    std::accumulate(grades.begin(), grades.end(), 0)
) / grades.size();

std::cout << "Средна оценка на успешните: " << avg;
// → Средна оценка на успешните: 84
```

---

## 10. FP срещу OOP — кога кое?

| Критерий | OOP | FP |
|---|---|---|
| **Моделиране** | Обекти с поведение и състояние | Трансформации на данни |
| **Мутация** | Честа (методи променят полета) | Избягвана (нови стойности) |
| **Тестване** | Изисква setup на обекти | Просто — чиста функция = лесен тест |
| **Паралелизъм** | Труден (shared mutable state) | Лесен (без споделено мутиращо се с.) |
| **Четимост** | Добра за домейн модели | Добра за обработка на данни |
| **Подходящ за** | GUI, игри, бизнес логика | Обработка на данни, алгоритми |

---

### Комбинирай OOP и FP

C++ позволява **хибриден подход** — и двете парадигми в едно.

```cpp
class OrderProcessor {
public:
    // OOP: клас управлява зависимостите
    explicit OrderProcessor(Database& db) : db(db) {}

    // FP: чиста функция за изчисление
    static double calculateTotal(const std::vector<Item>& items) {
        return std::accumulate(items.begin(), items.end(), 0.0,
            [](double sum, const Item& item) {
                return sum + item.price * item.quantity;
            });
    }

    // OOP: метод с страничен ефект (запис в БД)
    void saveOrder(const Order& order) {
        db.insert(order); // страничен ефект — изолиран тук
    }

private:
    Database& db;
};
```

---

## Резюме

```
Функционално Програмиране в C++

Чисти функции     → предвидими, лесно тестваеми
Неизменяемост     → const, работа с копия
Ламбди            → анонимни функции, capture
Функции от пo-висок ред → приема/връща функции
std::transform    → map  (прилага функция върху елементи)
std::copy_if      → filter (запазва елементи по условие)
std::accumulate   → reduce (свива до стойност)
Функтори          → обекти с operator()
Композиция        → изгражда сложно от просто

Принцип:
  Бизнес логика → чисти функции (FP)
  Структура и зависимости → класове (OOP)
  Странични ефекти → изолирани на границите
```

---

## Допълнителни ресурси

- `<algorithm>` — `std::transform`, `std::copy_if`, `std::sort`, `std::find_if`, ...
- `<numeric>` — `std::accumulate`, `std::partial_sum`, `std::iota`
- `<functional>` — `std::function`, `std::bind`, `std::ref`
- C++20 Ranges — по-четима FP верига: `nums | filter(...) | transform(...)`
