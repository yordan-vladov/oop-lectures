# Домашна работа 2: Thread-safe система за задачи

## Цел

Целта на задачата е да се проектира и имплементира многонишкова система за обработка на задачи (job queue), която демонстрира прилагането на **тестване** (Unit, Integration, Mocking) и **паралелност** (нишки, мютекси, атомарни типове, condition_variable) в C++.

---

## Контекст

Изграждате вътрешна система, която приема задачи (jobs), поставя ги в опашка и ги обработва паралелно от няколко работни нишки. Системата трябва да е напълно покрита с тестове.

---

## Изисквания към класовете

### 1) `Job` (5 точки)

Представлява единична задача за изпълнение.

**Членове:**
- `int id` — уникален идентификатор
- `std::string name` — описание на задачата
- `int priority` — приоритет (1 = нисък, 5 = висок)

**Методи:**
- Параметризиран конструктор
- Getters за всички полета
- `std::string toString() const` — връща `"Job(id=1, name=..., priority=3)"`
- Валидация: `priority` трябва да е между 1 и 5 включително; при невалидна стойност — хвърлете `std::invalid_argument`

---

### 2) `INotifier` — интерфейс (5 точки)

Абстрактен интерфейс за известяване при завършване на задача.

```cpp
class INotifier {
public:
    virtual void onJobCompleted(int jobId, const std::string& jobName) = 0;
    virtual ~INotifier() = default;
};
```

Имплементирайте:
- `ConsoleNotifier` — извежда съобщение на `std::cout`
- `MockNotifier` — записва извикванията (за тестване):
  - `int getCallCount() const`
  - `int getLastJobId() const`
  - `std::string getLastJobName() const`

---

### 3) `JobQueue` — thread-safe опашка (15 точки)

Thread-safe опашка, достъпвана от много нишки.

**Изисквания:**
- Вътрешно използва `std::queue<Job>` и `std::mutex` (капсулирани)
- `void push(const Job& job)` — добавя задача
- `bool tryPop(Job& outJob)` — изважда задача; връща `false` ако опашката е празна
- `bool empty() const`
- `std::size_t size() const`
- `void waitAndPop(Job& outJob)` — блокира с `std::condition_variable` докато не се появи задача **или** не е сигнализирано спиране
- `void signalStop()` — уведомява всички чакащи нишки да спрат
- `bool isStopped() const`

> Мютексът трябва да е `mutable`, за да можете да заключвате в `const` методи.

---

### 4) `WorkerPool` — пул от работни нишки (15 точки)

Пул от N работни нишки, всяка от които взима задачи от `JobQueue` и ги обработва.

**Изисквания:**
- Конструктор: `WorkerPool(int numThreads, JobQueue& queue, INotifier& notifier)`
- `void start()` — стартира нишките
- `void stop()` — спира нишките (сигнализира `JobQueue` и изчаква всички нишки с `join()`)
- `int getProcessedCount() const` — брой обработени задачи (използвайте `std::atomic<int>`)
- Всяка нишка при обработка на задача извиква `notifier.onJobCompleted(...)`

---

## Изисквания към тестовете (30 точки)

Напишете тестове в отделни `.cpp` файлове. Всеки тест следва **AAA структурата** (Arrange / Act / Assert). Ползвайте `assert` и `std::cout << "[PASS] ..."`.

### Unit тестове — `test_unit.cpp` (10 точки)

Тествайте `Job` и `JobQueue` **изолирано** (без нишки):

1. Конструиране на `Job` с валидни данни — `toString()` връща правилен низ
2. Конструиране на `Job` с `priority = 0` — хвърля `std::invalid_argument`
3. Конструиране на `Job` с `priority = 6` — хвърля `std::invalid_argument`
4. `JobQueue::push` + `size()` — размерът се увеличава правилно
5. `JobQueue::tryPop` на празна опашка — връща `false`
6. `JobQueue::push` + `tryPop` — успешно извлича задачата

### Integration тестове — `test_integration.cpp` (10 точки)

Тествайте взаимодействието `WorkerPool` + `JobQueue` + `MockNotifier` с реални нишки:

1. Пул от 2 нишки обработва 6 задачи — след `stop()` `getProcessedCount() == 6`
2. `MockNotifier::getCallCount()` е равен на броя задачи след приключване
3. При празна опашка и `signalStop()` — нишките спират без блокиране
4. Добавяне на задачи след `start()` (а не преди) — всички се обработват

### Mocking тест — `test_mock.cpp` (10 точки)

Тествайте `WorkerPool` с `MockNotifier`:

1. `onJobCompleted` е извикан с правилното `jobId` и `jobName`
2. Notifier-ът **не е** извикан при нула задачи
3. При 1 задача — `getCallCount() == 1` и `getLastJobId()` съвпада

---

## Примерна употреба (`main.cpp`)

```cpp
#include "Job.h"
#include "JobQueue.h"
#include "WorkerPool.h"
#include "ConsoleNotifier.h"
#include <iostream>

int main() {
    JobQueue queue;
    ConsoleNotifier notifier;
    WorkerPool pool(3, queue, notifier);

    pool.start();

    for (int i = 1; i <= 9; i++) {
        queue.push(Job(i, "задача_" + std::to_string(i), i % 5 + 1));
    }

    // Изчакваме малко, после спираме
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    pool.stop();

    std::cout << "Обработени задачи: " << pool.getProcessedCount() << "\n";
    return 0;
}
```

Примерен изход:
```
[Нишка 0] Обработена: задача_1 (id=1)
[Нишка 2] Обработена: задача_2 (id=2)
[Нишка 1] Обработена: задача_3 (id=3)
...
Обработени задачи: 9
```

---

## Структура на проекта

```
homework_2/
├── Job.h
├── INotifier.h
├── ConsoleNotifier.h
├── MockNotifier.h
├── JobQueue.h
├── WorkerPool.h
├── main.cpp
├── test_unit.cpp
├── test_integration.cpp
└── test_mock.cpp
```

---

## Оценяване

| Компонент                          | Точки |
|------------------------------------|-------|
| `Job` (конструктор, валидация, toString) | 5  |
| `INotifier` + `MockNotifier`       | 5     |
| `JobQueue` (thread-safe, condition_variable) | 15 |
| `WorkerPool` (нишки, atomic брояч) | 15    |
| Unit тестове                       | 10    |
| Integration тестове                | 10    |
| Mocking тестове                    | 10    |
| **Общо**                           | **70**|

---

## Правила и насоки

- Използвайте `std::lock_guard` за прости заключвания и `std::unique_lock` там, където е необходимо (напр. с `condition_variable`).
- Мютексът в `JobQueue` трябва да е `mutable`.
- `WorkerPool::getProcessedCount()` трябва да е thread-safe — използвайте `std::atomic<int>`.
- Не ползвайте глобални мютекси — капсулирайте ги в класовете.
- Всеки тест следва **AAA** (Arrange / Act / Assert).
- Тестовете с нишки трябва да викат `pool.stop()` и `pool.join()` преди `assert`-овете.

---

## Компилация

```bash
# Основна програма
g++ -std=c++17 -pthread main.cpp -o main

# Unit тестове
g++ -std=c++17 -pthread test_unit.cpp -o test_unit

# Integration тестове
g++ -std=c++17 -pthread test_integration.cpp -o test_integration

# Mock тестове
g++ -std=c++17 -pthread test_mock.cpp -o test_mock
```

---

## Какво да предадете

- Линк към GitHub хранилище, което да включва:
    * Изходен код (`.h`/`.cpp`)
    * `README.md`, което да включва:
        * как се компилира (напр. `g++ -std=c++17 -pthread main.cpp -o main`),
        * кратко описание на класовете,
        * примерен изход от програмата.
