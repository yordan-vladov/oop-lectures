# Паралелност и многонишково програмиране (Concurrency & Multithreading)

---

## Съдържание

1. Какво е паралелност?
2. Нишки в C++ (`std::thread`)
3. Споделено състояние и race conditions
4. Мютекси и синхронизация
5. Заключващи обекти (RAII за мютекси)
6. `std::atomic`
7. Условни променливи (`std::condition_variable`)
8. Thread-safe класове в ООП
9. Обобщение

---

## 1. Какво е паралелност?

**Паралелност** означава няколко задачи да се изпълняват едновременно (или привидно едновременно).

### Защо ни трябва?

- **Производителност** - модерните процесори имат много ядра
- **Отзивчивост** - UI не замръзва докато се изпълнява тежка задача
- **I/O операции** - докато чакаме мрежа/диск, правим друга работа

---

### Паралелност vs Конкурентност

```
Конкурентност (Concurrency):          Паралелност (Parallelism):
Задачи се редуват на едно ядро        Задачи текат едновременно на много ядра

Ядро 1: A--B--A--B--A                 Ядро 1: A--A--A--A--A
                                      Ядро 2: B--B--B--B--B
```

> C++ `std::thread` поддържа и двете - OS решава дали да ги изпълни на едно или много ядра.

---

## 2. Нишки в C++ (`std::thread`)

Нишките са обекти от клас `std::thread` (заглавен файл `<thread>`).

### Създаване на нишка

```cpp
#include <iostream>
#include <thread>

void printMessage(const std::string& msg) {
    std::cout << msg << "\n";
}

int main() {
    std::thread t(printMessage, "Здравей от нишка!");
    t.join(); // изчакваме нишката да завърши
    std::cout << "Нишката завърши.\n";
    return 0;
}
```

---

### Ламбда функции в нишки

```cpp
#include <iostream>
#include <thread>

int main() {
    int x = 42;

    std::thread t([x]() {
        std::cout << "Стойността е: " << x << "\n";
    });

    t.join();
    return 0;
}
```

---

### `join` vs `detach`

```cpp
#include <iostream>
#include <thread>

void task() {
    std::cout << "Задача изпълнена.\n";
}

int main() {
    std::thread t1(task);
    t1.join();    // ← изчакваме нишката (безопасно)

    std::thread t2(task);
    t2.detach();  // ← пускаме я "свободна" (рядко се ползва)

    return 0;     // ⚠️ ако t2 не е завършила, поведението е недефинирано!
}
```

> **Правило**: Почти винаги ползвайте `join()`. `detach()` е рядко оправдано.

---

### Множество нишки

```cpp
#include <iostream>
#include <thread>
#include <vector>

void worker(int id) {
    std::cout << "Нишка " << id << " работи.\n";
}

int main() {
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; i++) {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Всички нишки завършиха.\n";
    return 0;
}
```

---

### Примерен изход (редът варира!)

```
Нишка 2 работи.
Нишка 0 работи.
Нишка 4 работи.
Нишка 1 работи.
Нишка 3 работи.
Всички нишки завършиха.
```

> **Важно**: Редът на изпълнение на нишките е **недетерминиран** - OS решава кога да ги изпълни.

---

## 3. Споделено състояние и race conditions

**Race condition** (надпревара) е грешка, при която резултатът зависи от непредвидимия ред на изпълнение на нишките.

---

### Пример: Брояч без синхронизация

```cpp
#include <iostream>
#include <thread>
#include <vector>

int counter = 0; // споделена променлива

void increment() {
    for (int i = 0; i < 10000; i++) {
        counter++; // ⚠️ НЕ е атомарна операция!
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(increment);
    }
    for (auto& t : threads) t.join();

    std::cout << "Очакван резултат: 100000\n";
    std::cout << "Действителен резултат: " << counter << "\n"; // ← може да е по-малко!
    return 0;
}
```

---

### Изход (недетерминиран)

```
Очакван резултат: 100000
Действителен резултат: 73482   ← грешка заради race condition!
```

---

### Защо `counter++` не е безопасна?

`counter++` всъщност са **три операции**:

```
1. Прочети counter в регистър  (LOAD)
2. Добави 1                    (ADD)
3. Запиши обратно              (STORE)
```

Ако две нишки изпълнят `LOAD` едновременно, едното записване ще бъде изгубено:

```
Нишка 1:  LOAD(5)   ADD → 6   STORE(6)
Нишка 2:  LOAD(5)   ADD → 6   STORE(6)   ← трябваше да е 7!
```

---

## 4. Мютекси и синхронизация

**Мютекс** (mutual exclusion) гарантира, че само **една нишка** може да изпълни критичен участък от код в даден момент.

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

int counter = 0;
std::mutex mtx; // мютексът пази counter

void increment() {
    for (int i = 0; i < 10000; i++) {
        mtx.lock();
        counter++; // критичен участък - само 1 нишка наведнъж
        mtx.unlock();
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(increment);
    }
    for (auto& t : threads) t.join();

    std::cout << "Резултат: " << counter << "\n"; // винаги 100000
    return 0;
}
```

---

### Изход

```
Резултат: 100000
```

---

### Проблемът с ръчен `lock/unlock`

```cpp
void riskyFunction() {
    mtx.lock();
    doSomething(); // ако хвърли изключение → mtx.unlock() никога не се извиква!
    mtx.unlock();  // ← може да не се достигне
}
```

> **Дедлок**: ако забравим `unlock()` или хвърлим изключение, нишките блокират завинаги.

---

## 5. Заключващи обекти (RAII за мютекси)

**RAII принципът** (Resource Acquisition Is Initialization) се прилага и за мютекси - обектът заключва при създаване и отключва при унищожаване.

---

### `std::lock_guard`

```cpp
#include <mutex>

std::mutex mtx;

void safeIncrement(int& counter) {
    std::lock_guard<std::mutex> lock(mtx); // ← заключва тук
    counter++;
    // ← автоматично отключва при излизане от scope (дори при изключение)
}
```

---

### `std::unique_lock` - по-гъвкав вариант

```cpp
#include <mutex>

std::mutex mtx;

void flexibleFunction(int& counter) {
    std::unique_lock<std::mutex> lock(mtx);

    counter++;

    lock.unlock(); // можем ръчно да отключим по-рано
    doLongOperation(); // това не е в критичния участък

    lock.lock(); // и да заключим отново ако трябва
    counter++;
}
```

---

### `lock_guard` vs `unique_lock`

| | `lock_guard` | `unique_lock` |
|---|---|---|
| **Прости случаи** | ✓ Препоръчан | Излишен overhead |
| **Ръчен unlock** | ✗ Не може | ✓ Може |
| **С condition_variable** | ✗ Не може | ✓ Задължителен |
| **Overhead** | Минимален | Малко по-голям |

> **Правило**: Използвайте `lock_guard` по подразбиране. Превключете на `unique_lock` само при нужда.

---

## 6. `std::atomic`

За прости операции (четене/писане на число) `std::atomic` е **по-бърза алтернатива** на мютекс.

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

std::atomic<int> counter{0}; // атомарна целочислена стойност

void increment() {
    for (int i = 0; i < 10000; i++) {
        counter++; // атомарна операция - без мютекс!
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(increment);
    }
    for (auto& t : threads) t.join();

    std::cout << "Резултат: " << counter << "\n"; // винаги 100000
    return 0;
}
```

---

### Кога да ползваме `atomic` vs `mutex`?

| Ситуация | Препоръка |
|---|---|
| Четене/писане на едно число/флаг | `std::atomic` |
| Защита на сложна структура от данни | `std::mutex` |
| Операция върху няколко свързани стойности | `std::mutex` |
| Висока производителност, прост тип | `std::atomic` |

---

## 7. Условни променливи (`std::condition_variable`)

**Условна променлива** позволява на нишка да **чака** докато не е изпълнено определено условие, вместо да проверява непрекъснато (busy-waiting).

---

### Пример: Producer-Consumer

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>

std::queue<std::string> taskQueue;
std::mutex mtx;
std::condition_variable cv;
bool done = false;

void producer() {
    std::vector<std::string> tasks = {"задача_1", "задача_2", "задача_3"};
    for (const auto& task : tasks) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            taskQueue.push(task);
            std::cout << "[Producer] Добавена: " << task << "\n";
        }
        cv.notify_one(); // събуди consumer нишката
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        done = true;
    }
    cv.notify_one();
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return !taskQueue.empty() || done; }); // чакай докато има задача

        while (!taskQueue.empty()) {
            std::string task = taskQueue.front();
            taskQueue.pop();
            lock.unlock();
            std::cout << "[Consumer] Обработена: " << task << "\n";
            lock.lock();
        }

        if (done) break;
    }
}

int main() {
    std::thread prod(producer);
    std::thread cons(consumer);
    prod.join();
    cons.join();
    return 0;
}
```

---

### Изход

```
[Producer] Добавена: задача_1
[Producer] Добавена: задача_2
[Consumer] Обработена: задача_1
[Producer] Добавена: задача_3
[Consumer] Обработена: задача_2
[Consumer] Обработена: задача_3
```

---

## 8. Thread-safe класове в ООП

Когато проектираме класове за многонишкова среда, мютексите и атомарните типове трябва да са **капсулирани** в класа.

---

### Thread-safe стек - `ThreadSafeStack.h`

```cpp
#pragma once
#include <stack>
#include <mutex>
#include <stdexcept>

template<typename T>
class ThreadSafeStack {
    std::stack<T> data;
    mutable std::mutex mtx; // mutable → може да се заключва в const методи

public:
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        data.push(value);
    }

    T pop() {
        std::lock_guard<std::mutex> lock(mtx);
        if (data.empty()) throw std::runtime_error("Стекът е празен");
        T value = data.top();
        data.pop();
        return value;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return data.empty();
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return data.size();
    }
};
```

---

### Употреба на `ThreadSafeStack`

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include "ThreadSafeStack.h"

ThreadSafeStack<int> stack;

void pusher(int start) {
    for (int i = start; i < start + 5; i++) {
        stack.push(i);
    }
}

void popper(int count) {
    for (int i = 0; i < count; i++) {
        try {
            int val = stack.pop();
            std::cout << "Извадено: " << val << "\n";
        } catch (const std::runtime_error&) {
            // стекът е бил празен - нормално в многонишкова среда
        }
    }
}

int main() {
    std::thread t1(pusher, 0);
    std::thread t2(pusher, 100);
    t1.join();
    t2.join();

    std::thread t3(popper, 5);
    std::thread t4(popper, 5);
    t3.join();
    t4.join();

    std::cout << "Останали в стека: " << stack.size() << "\n";
    return 0;
}
```

---

### Thread-safe брояч - `Counter.h`

```cpp
#pragma once
#include <atomic>

class Counter {
    std::atomic<int> value{0};

public:
    void increment() { value++; }
    void decrement() { value--; }
    void reset()     { value = 0; }
    int  get() const { return value.load(); }
};
```

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include "Counter.h"

Counter counter;

void incrementMany(int times) {
    for (int i = 0; i < times; i++) {
        counter.increment();
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(incrementMany, 10000);
    }
    for (auto& t : threads) t.join();

    std::cout << "Брояч: " << counter.get() << "\n"; // винаги 100000
    return 0;
}
```

---

### Thread-safe Singleton

Разпространен проблем: Singleton, достъпван от много нишки.

```cpp
#pragma once
#include <mutex>
#include <memory>

class Logger {
    static std::unique_ptr<Logger> instance;
    static std::mutex mtx;

    Logger() = default;

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance() {
        std::lock_guard<std::mutex> lock(mtx); // пази инициализацията
        if (!instance) {
            instance = std::unique_ptr<Logger>(new Logger());
        }
        return *instance;
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[LOG] " << message << "\n";
    }
};

std::unique_ptr<Logger> Logger::instance = nullptr;
std::mutex Logger::mtx;
```

---

### По-добре: `std::call_once` за Singleton

```cpp
#pragma once
#include <mutex>
#include <memory>

class Logger {
    static std::unique_ptr<Logger> instance;
    static std::once_flag initFlag; // гарантира еднократна инициализация

    Logger() = default;

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance() {
        std::call_once(initFlag, []() {
            instance = std::unique_ptr<Logger>(new Logger());
        });
        return *instance;
    }

    void log(const std::string& msg) {
        std::cout << "[LOG] " << msg << "\n";
    }
};

std::unique_ptr<Logger> Logger::instance = nullptr;
std::once_flag Logger::initFlag;
```

---

### Правила за thread-safe класове

```
✓ Капсулирай мютекса в класа - не го излагай навън
✓ Маркирай мютекса като mutable - за заключване в const методи
✓ Предпочитай lock_guard пред ръчен lock/unlock
✓ Минимизирай критичния участък - само необходимият код вътре
✓ Не извиквай външни функции вътре в lock - риск от дедлок
✓ За прости стойности - atomic е по-бърз от mutex
```

---

## Обобщение

**Паралелност** = множество нишки, изпълняващи се едновременно

**Основни инструменти в C++:**

| Инструмент | Употреба |
|---|---|
| `std::thread` | Създаване и управление на нишки |
| `std::mutex` | Изключващ достъп до споделени данни |
| `std::lock_guard` | RAII обвивка - автоматично unlock |
| `std::unique_lock` | По-гъвкав lock (с condition_variable) |
| `std::atomic<T>` | Атомарни операции без mutex |
| `std::condition_variable` | Чакане на условие между нишки |
| `std::call_once` | Еднократна инициализация |

---

**Основни проблеми и решения:**

| Проблем | Описание | Решение |
|---|---|---|
| **Race condition** | Нишки четат/пишат едновременно | `mutex` или `atomic` |
| **Deadlock** | Нишки чакат една друга | RAII (`lock_guard`), правилен ред на lock |
| **Забравен unlock** | `lock()` без `unlock()` | `lock_guard` (RAII) |
| **Busy-waiting** | Нишка проверява непрекъснато | `condition_variable` |

---

**ООП принципи при паралелност:**

| Принцип | Приложение |
|---|---|
| **Encapsulation** | Мютексът е частен член на класа |
| **Single Responsibility** | Класът управлява собствената си синхронизация |
| **RAII** | `lock_guard` освобождава ресурса при destructor |
| **DIP** | Синхронизацията е скрита зад интерфейс |
