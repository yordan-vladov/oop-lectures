# Тестване (Testing)

---

## Съдържание

1. Защо тестваме?
2. Нива на тестване
3. Unit тестване
4. Integration тестване
5. System тестване
6. Acceptance тестване
7. Test Driven Development (TDD)
8. Mocking
9. Обобщение

---

## 1. Защо тестваме?

**Тестването** е процес на проверка, че кодът работи правилно - сега и след бъдещи промени.

- **Намираме грешки рано** - по-евтино е да поправим бъг сега, отколкото след deploy
- **Документация** - тестовете показват как се очаква да работи кодът
- **Refactoring с увереност** - можем да променяме код без страх, че ще счупим нещо
- **Дизайн на кода** - трудно тестваният код обикновено е лошо проектиран

> **Принцип**: Ако кодът е трудно тестваем, той е трудно разбираем и поддържаем.

---

### Цена на бъговете

| Намерен при    | Относителна цена |
| -------------- | ---------------- |
| Писане на код  | 1x               |
| Code review    | 5x               |
| Unit тест      | 10x              |
| Integration    | 15x              |
| Acceptance     | 25x              |
| Продукция      | **100x**         |

---

## 2. Нива на тестване

![](https://media.licdn.com/dms/image/v2/D4D12AQEgb-wIv7V5qw/article-cover_image-shrink_720_1280/article-cover_image-shrink_720_1280/0/1690460596841?e=2147483647&v=beta&t=kdavaNX3WDloHC3VqiySBhBKo48eNu1lLAUBqK4hDIc)

---


| Ниво          | Тества                          | Скорост | Брой    |
| ------------- | ------------------------------- | ------- | ------- |
| **Unit**      | Отделен клас/метод              | Бърз    | Много   |
| **Integration** | Взаимодействие между компоненти | Среден  | Средно  |
| **System**    | Цялата система като едно цяло   | Бавен   | Малко   |
| **Acceptance (E2E)** | Изискванията на потребителя | Много бавен | Много малко |

---

## 3. Unit тестване

**Unit тест** проверява една единица код (клас, метод) **изолирано** от останалата система.

### Характеристики:
- Тества **едно нещо** наведнъж
- **Бърз** - изпълнява се за милисекунди
- **Независим** - не зависи от база данни, мрежа, файлова система
- **Детерминистичен** - при едни и същи входни данни дава еднакъв резултат

---

### Примерен клас - `BankAccount.h`

```cpp
#pragma once
#include <stdexcept>

class BankAccount {
    double balance;
    std::string owner;

public:
    BankAccount(const std::string& owner, double initialBalance = 0.0)
        : owner(owner), balance(initialBalance) {
        if (initialBalance < 0) {
            throw std::invalid_argument("Началното салдо не може да е отрицателно");
        }
    }

    void deposit(double amount) {
        if (amount <= 0) throw std::invalid_argument("Сумата трябва да е положителна");
        balance += amount;
    }

    void withdraw(double amount) {
        if (amount <= 0) throw std::invalid_argument("Сумата трябва да е положителна");
        if (amount > balance) throw std::runtime_error("Недостатъчно средства");
        balance -= amount;
    }

    double getBalance() const { return balance; }
    const std::string& getOwner() const { return owner; }
};
```

---

### Unit тестове без framework - `test_bank_account.cpp`

```cpp
#include "BankAccount.h"
#include <iostream>
#include <cassert>
#include <stdexcept>

// Прост тест - проверява едно конкретно поведение
void test_deposit_increases_balance() {
    BankAccount account("Алис", 100.0);
    account.deposit(50.0);
    assert(account.getBalance() == 150.0);
    std::cout << "[PASS] deposit увеличава баланса\n";
}

void test_withdraw_decreases_balance() {
    BankAccount account("Алис", 100.0);
    account.withdraw(30.0);
    assert(account.getBalance() == 70.0);
    std::cout << "[PASS] withdraw намалява баланса\n";
}

void test_withdraw_throws_on_insufficient_funds() {
    BankAccount account("Алис", 50.0);
    try {
        account.withdraw(100.0);
        assert(false); // трябваше да хвърли изключение
    } catch (const std::runtime_error& e) {
        std::cout << "[PASS] withdraw хвърля при недостатъчни средства\n";
    }
}

void test_negative_initial_balance_throws() {
    try {
        BankAccount account("Алис", -10.0);
        assert(false);
    } catch (const std::invalid_argument&) {
        std::cout << "[PASS] конструктор хвърля при отрицателно начално салдо\n";
    }
}

int main() {
    test_deposit_increases_balance();
    test_withdraw_decreases_balance();
    test_withdraw_throws_on_insufficient_funds();
    test_negative_initial_balance_throws();
    std::cout << "\nВсички unit тестове преминаха!\n";
    return 0;
}
```

---

### Изход

```
[PASS] deposit увеличава баланса
[PASS] withdraw намалява баланса
[PASS] withdraw хвърля при недостатъчни средства
[PASS] конструктор хвърля при отрицателно начално салдо

Всички unit тестове преминаха!
```

---

### Структурата AAA (Arrange, Act, Assert)

Всеки добър unit тест следва три стъпки:

```cpp
void test_deposit_increases_balance() {
    // Arrange - подготвяме обектите
    BankAccount account("Боби", 100.0);

    // Act - изпълняваме действието
    account.deposit(50.0);

    // Assert - проверяваме резултата
    assert(account.getBalance() == 150.0);
}
```

> **Правило**: Ако тестът ти има повече от едно `assert`, той вероятно тества повече от едно нещо.

---

## 4. Integration тестване

**Integration тест** проверява как **няколко компонента** работят заедно.

### Характеристики:
- Тества **взаимодействието** между компоненти
- Може да включва **реални зависимости** (файлове, база данни)
- **По-бавен** от unit тест
- Открива **проблеми в интерфейсите** между компоненти

---

### Пример - класове за интеграция

```cpp
// Logger.h - компонент 1
#pragma once
#include <string>
#include <vector>

class Logger {
    std::vector<std::string> logs;
public:
    void log(const std::string& message) {
        logs.push_back(message);
    }
    const std::vector<std::string>& getLogs() const { return logs; }
    bool contains(const std::string& msg) const {
        for (const auto& log : logs) {
            if (log.find(msg) != std::string::npos) return true;
        }
        return false;
    }
};

// TransactionService.h - компонент 2, зависи от BankAccount и Logger
#pragma once
#include "BankAccount.h"
#include "Logger.h"

class TransactionService {
    Logger& logger;
public:
    explicit TransactionService(Logger& logger) : logger(logger) {}

    bool transfer(BankAccount& from, BankAccount& to, double amount) {
        try {
            from.withdraw(amount);
            to.deposit(amount);
            logger.log("Превод: " + std::to_string(amount) +
                       " от " + from.getOwner() +
                       " към " + to.getOwner());
            return true;
        } catch (const std::exception& e) {
            logger.log("ГРЕШКА: " + std::string(e.what()));
            return false;
        }
    }
};
```

---

### Integration тест

```cpp
#include "BankAccount.h"
#include "TransactionService.h"
#include "Logger.h"
#include <iostream>
#include <cassert>

// Тества взаимодействието: BankAccount + TransactionService + Logger
void test_successful_transfer_updates_both_accounts_and_logs() {
    // Arrange
    BankAccount alice("Алис", 500.0);
    BankAccount bob("Боби", 100.0);
    Logger logger;
    TransactionService service(logger);

    // Act
    bool success = service.transfer(alice, bob, 200.0);

    // Assert - проверяваме и двата акаунта И логовете
    assert(success == true);
    assert(alice.getBalance() == 300.0);
    assert(bob.getBalance() == 300.0);
    assert(logger.contains("Превод"));
    assert(logger.contains("Алис"));
    assert(logger.contains("Боби"));
    std::cout << "[PASS] успешен превод актуализира и двата акаунта и логва\n";
}

void test_failed_transfer_does_not_change_balances() {
    // Arrange
    BankAccount alice("Алис", 50.0);
    BankAccount bob("Боби", 100.0);
    Logger logger;
    TransactionService service(logger);

    // Act
    bool success = service.transfer(alice, bob, 200.0); // недостатъчно средства

    // Assert - нищо не трябва да се е променило
    assert(success == false);
    assert(alice.getBalance() == 50.0);
    assert(bob.getBalance() == 100.0);
    assert(logger.contains("ГРЕШКА"));
    std::cout << "[PASS] неуспешен превод не променя балансите\n";
}

int main() {
    test_successful_transfer_updates_both_accounts_and_logs();
    test_failed_transfer_does_not_change_balances();
    std::cout << "\nВсички integration тестове преминаха!\n";
    return 0;
}
```

---

### Unit vs Integration - сравнение

| | Unit тест | Integration тест |
| --- | --- | --- |
| **Тества** | `BankAccount` изолирано | `BankAccount` + `TransactionService` + `Logger` заедно |
| **Въпрос** | "Withdraw работи ли?" | "Преводът актуализира ли и двата акаунта и пише ли в лога?" |

---

## 5. System тестване

**System тест** проверява **цялата система като едно цяло** - всички компоненти са реални и работят заедно, но тестът е автоматизиран и не изисква потребител.

### Характеристики:
- Тества **пълния поток** от заявка до отговор
- Всички зависимости са **реални** (без mock-ове)
- Проверява **нефункционални изисквания** - производителност, лимити, грешки на системно ниво
- По-бавен от integration тест, но по-бърз от ръчен acceptance тест

---

### Разлика от Integration тест

| | Integration тест | System тест |
| --- | --- | --- |
| **Обхват** | 2-3 компонента заедно | Цялата система |
| **Зависимости** | Могат да са mock-ове | Само реални компоненти |
| **Въпрос** | "TransactionService + Logger работят ли заедно?" | "Банковата система приема ли превод от край до край?" |

---

### Пример - система с `Bank` фасада

```cpp
// Bank.h - фасада, обединява цялата система
#pragma once
#include "BankAccount.h"
#include "TransactionService.h"
#include "Logger.h"
#include <unordered_map>
#include <stdexcept>

class Bank {
    std::unordered_map<int, BankAccount*> accounts;
    Logger logger;
    TransactionService transactionService;
    int nextId = 1;

public:
    Bank() : transactionService(logger) {}

    int openAccount(const std::string& owner, double initialBalance = 0.0) {
        int id = nextId++;
        accounts[id] = new BankAccount(owner, initialBalance);
        logger.log("Открита сметка #" + std::to_string(id) + " за " + owner);
        return id;
    }

    bool transfer(int fromId, int toId, double amount) {
        auto itFrom = accounts.find(fromId);
        auto itTo   = accounts.find(toId);
        if (itFrom == accounts.end() || itTo == accounts.end()) {
            throw std::invalid_argument("Невалиден номер на сметка");
        }
        return transactionService.transfer(*itFrom->second, *itTo->second, amount);
    }

    double getBalance(int accountId) const {
        auto it = accounts.find(accountId);
        if (it == accounts.end()) throw std::invalid_argument("Невалиден номер на сметка");
        return it->second->getBalance();
    }

    const Logger& getLogger() const { return logger; }

    ~Bank() { for (auto& [id, acc] : accounts) delete acc; }
};
```

---

### System тест

```cpp
#include "Bank.h"
#include <iostream>
#include <cassert>

// System тест: тества цялата банкова система като едно цяло
void system_test_full_transfer_flow() {
    // Arrange - реална система, без mock-ове
    Bank bank;
    int aliceId = bank.openAccount("Алис", 1000.0);
    int bobId   = bank.openAccount("Боби", 200.0);

    // Act - пълен поток
    bool result = bank.transfer(aliceId, bobId, 300.0);

    // Assert - проверяваме крайното състояние на системата
    assert(result == true);
    assert(bank.getBalance(aliceId) == 700.0);
    assert(bank.getBalance(bobId)   == 500.0);
    assert(bank.getLogger().contains("Превод"));
    std::cout << "[PASS] пълен поток на превод работи коректно\n";
}

void system_test_invalid_account_throws() {
    Bank bank;
    bank.openAccount("Алис", 500.0);

    try {
        bank.transfer(1, 999, 100.0); // несъществуваща сметка
        assert(false);
    } catch (const std::invalid_argument&) {
        std::cout << "[PASS] системата отхвърля превод към невалидна сметка\n";
    }
}

void system_test_multiple_transfers_consistent_state() {
    Bank bank;
    int aliceId = bank.openAccount("Алис", 1000.0);
    int bobId   = bank.openAccount("Боби", 0.0);
    int carolId = bank.openAccount("Карол", 0.0);

    bank.transfer(aliceId, bobId,   400.0);
    bank.transfer(aliceId, carolId, 300.0);
    bank.transfer(bobId,   carolId, 100.0);

    // Общата сума трябва да е запазена: 1000 = 300 + 300 + 400
    double total = bank.getBalance(aliceId)
                 + bank.getBalance(bobId)
                 + bank.getBalance(carolId);
    assert(total == 1000.0);
    std::cout << "[PASS] многократни преводи запазват консистентно състояние\n";
}

int main() {
    system_test_full_transfer_flow();
    system_test_invalid_account_throws();
    system_test_multiple_transfers_consistent_state();
    std::cout << "\nВсички system тестове преминаха!\n";
    return 0;
}
```

---

### Изход

```
[PASS] пълен поток на превод работи коректно
[PASS] системата отхвърля превод към невалидна сметка
[PASS] многократни преводи запазват консистентно състояние

Всички system тестове преминаха!
```

---

## 6. Acceptance тестване

**Acceptance тест** проверява дали системата покрива **изискванията на потребителя**.

### Характеристики:
- Написан от гледна точка на **потребителя**, не програмиста
- Проверява **цели сценарии** (use cases)
- Използва **истинска система** (без mock-ове)
- Формулирани като: **"Дадено... Когато... Тогава..."** (Given/When/Then)

---

### Примерен сценарий - `acceptance_tests.cpp`

```cpp
#include "BankAccount.h"
#include "TransactionService.h"
#include "Logger.h"
#include <iostream>
#include <cassert>

// Acceptance тест: потребителска история
// "Като клиент на банка, искам да мога да прехвърлям пари
//  към друга сметка, като се проверява дали имам достатъчно средства."

void scenario_client_transfers_money_successfully() {
    std::cout << "Сценарий: Клиент прехвърля пари успешно\n";

    // ДАДЕНО - клиент с достатъчно средства
    BankAccount clientAccount("Клиент", 1000.0);
    BankAccount recipientAccount("Получател", 0.0);
    Logger logger;
    TransactionService bank(logger);

    // КОГАТО - клиентът нарежда превод
    bool result = bank.transfer(clientAccount, recipientAccount, 500.0);

    // ТОГАВА - преводът е успешен и средствата са прехвърлени
    assert(result == true);
    assert(clientAccount.getBalance() == 500.0);
    assert(recipientAccount.getBalance() == 500.0);

    std::cout << "  [PASS] Преводът е успешен\n";
    std::cout << "  [PASS] Клиентът има 500 лв.\n";
    std::cout << "  [PASS] Получателят има 500 лв.\n\n";
}

void scenario_client_cannot_overdraw_account() {
    std::cout << "Сценарий: Клиент не може да изтегли повече от наличното\n";

    // ДАДЕНО - клиент с 200 лв.
    BankAccount clientAccount("Клиент", 200.0);
    BankAccount recipientAccount("Получател", 0.0);
    Logger logger;
    TransactionService bank(logger);

    // КОГАТО - клиентът се опитва да преведе 500 лв.
    bool result = bank.transfer(clientAccount, recipientAccount, 500.0);

    // ТОГАВА - операцията е отказана, нищо не е променено
    assert(result == false);
    assert(clientAccount.getBalance() == 200.0);
    assert(recipientAccount.getBalance() == 0.0);

    std::cout << "  [PASS] Преводът е отказан\n";
    std::cout << "  [PASS] Балансите са непроменени\n\n";
}

int main() {
    scenario_client_transfers_money_successfully();
    scenario_client_cannot_overdraw_account();
    std::cout << "Всички acceptance тестове преминаха!\n";
    return 0;
}
```

---

### Изход

```
Сценарий: Клиент прехвърля пари успешно
  [PASS] Преводът е успешен
  [PASS] Клиентът има 500 лв.
  [PASS] Получателят има 500 лв.

Сценарий: Клиент не може да изтегли повече от наличното
  [PASS] Преводът е отказан
  [PASS] Балансите са непроменени

Всички acceptance тестове преминаха!
```

---

## 7. Test Driven Development (TDD)

**TDD** е методология, при която **пишем тестовете преди кода**.

### Цикълът Red-Green-Refactor

![](https://miro.medium.com/v2/resize:fit:750/0*v_kEukCKDOPhTqFI.png)

---

### TDD на практика: Имплементиране на `ShoppingCart`

**Стъпка 1: RED - пишем теста (класът не съществува)**

```cpp
// test_shopping_cart.cpp
#include "ShoppingCart.h"  // ← още не съществува!
#include <cassert>

void test_new_cart_is_empty() {
    ShoppingCart cart;
    assert(cart.getItemCount() == 0);
    assert(cart.getTotal() == 0.0);
}

int main() {
    test_new_cart_is_empty();  // ← КОМПИЛАЦИОННА ГРЕШКА (RED)
}
```

---

**Стъпка 2: GREEN - минималната имплементация**

```cpp
// ShoppingCart.h - пишем само толкова, колкото трябва теста да мине
#pragma once

class ShoppingCart {
public:
    int getItemCount() const { return 0; }
    double getTotal() const { return 0.0; }
};
```

Тестът минава. Сега добавяме следващия тест:

```cpp
void test_add_item_increases_count() {
    ShoppingCart cart;
    cart.addItem("Лаптоп", 1500.0);
    assert(cart.getItemCount() == 1);   // RED - addItem не съществува
}
```

---

**Стъпка 3: GREEN за новия тест**

```cpp
#pragma once
#include <vector>
#include <string>

class ShoppingCart {
    struct Item { std::string name; double price; };
    std::vector<Item> items;

public:
    void addItem(const std::string& name, double price) {
        items.push_back({name, price});
    }

    int getItemCount() const { return items.size(); }

    double getTotal() const {
        double total = 0.0;
        for (const auto& item : items) total += item.price;
        return total;
    }
};
```

---

**Стъпка 4: Нов тест за remove**

```cpp
void test_remove_item_decreases_count() {
    ShoppingCart cart;
    cart.addItem("Лаптоп", 1500.0);
    cart.addItem("Мишка", 25.0);
    cart.removeItem("Лаптоп");           // RED - removeItem не съществува
    assert(cart.getItemCount() == 1);
    assert(cart.getTotal() == 25.0);
}
```

---

**Стъпка 5: GREEN + REFACTOR**

```cpp
#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

class ShoppingCart {
    struct Item { std::string name; double price; };
    std::vector<Item> items;

public:
    void addItem(const std::string& name, double price) {
        if (price < 0) throw std::invalid_argument("Цената не може да е отрицателна");
        items.push_back({name, price});
    }

    void removeItem(const std::string& name) {
        auto it = std::find_if(items.begin(), items.end(),
            [&](const Item& i) { return i.name == name; });
        if (it == items.end()) throw std::runtime_error("Продуктът не е намерен: " + name);
        items.erase(it);
    }

    int getItemCount() const { return static_cast<int>(items.size()); }

    double getTotal() const {
        double total = 0.0;
        for (const auto& item : items) total += item.price;
        return total;
    }
};
```

---

### Всички TDD тестове заедно

```cpp
#include "ShoppingCart.h"
#include <iostream>
#include <cassert>

void test_new_cart_is_empty() {
    ShoppingCart cart;
    assert(cart.getItemCount() == 0);
    assert(cart.getTotal() == 0.0);
    std::cout << "[PASS] нова количка е празна\n";
}

void test_add_item_increases_count_and_total() {
    ShoppingCart cart;
    cart.addItem("Лаптоп", 1500.0);
    cart.addItem("Мишка", 25.0);
    assert(cart.getItemCount() == 2);
    assert(cart.getTotal() == 1525.0);
    std::cout << "[PASS] добавяне на продукт увеличава броя и сумата\n";
}

void test_remove_item_decreases_count() {
    ShoppingCart cart;
    cart.addItem("Лаптоп", 1500.0);
    cart.addItem("Мишка", 25.0);
    cart.removeItem("Лаптоп");
    assert(cart.getItemCount() == 1);
    assert(cart.getTotal() == 25.0);
    std::cout << "[PASS] премахване на продукт намалява броя\n";
}

void test_remove_nonexistent_item_throws() {
    ShoppingCart cart;
    try {
        cart.removeItem("Несъществуващ");
        assert(false);
    } catch (const std::runtime_error&) {
        std::cout << "[PASS] премахване на несъществуващ продукт хвърля изключение\n";
    }
}

int main() {
    test_new_cart_is_empty();
    test_add_item_increases_count_and_total();
    test_remove_item_decreases_count();
    test_remove_nonexistent_item_throws();
    std::cout << "\nВсички TDD тестове преминаха!\n";
    return 0;
}
```

---

### Предимства на TDD

```
✓ Дизайн     - принуждава ни да мислим за интерфейса ПРЕДИ имплементацията
✓ Покритие   - всеки ред код има тест (написан преди кода)
✓ Документация - тестовете показват как се ползва класът
✓ Увереност  - можем да рефакторираме без страх
✓ Фокус      - пишем само необходимия код (YAGNI)
```

---

## 8. Mocking

**Mock обект** е заместител на реална зависимост, който:
- Имитира поведението на истинския обект
- Позволява ни да тестваме **изолирано**
- Дава контрол над **какво връща** зависимостта

---

### Проблемът без Mock

```cpp
// PaymentGateway.h - реална зависимост (изпраща HTTP заявки, струва пари!)
class RealPaymentGateway {
public:
    bool charge(const std::string& cardNumber, double amount) {
        // HTTP заявка към Stripe/PayPal → бавно, скъпо, нестабилно в тест
        std::cout << "Изпращане на реална транзакция...\n";
        return true;
    }
};

// OrderProcessor.h - зависи от PaymentGateway
class OrderProcessor {
    RealPaymentGateway gateway; // ← директна зависимост = трудно тестване
public:
    bool processOrder(const std::string& card, double amount) {
        return gateway.charge(card, amount);
    }
};
```

---

### Решение: Интерфейс + Mock

```cpp
// IPaymentGateway.h - абстрактен интерфейс
#pragma once
#include <string>

class IPaymentGateway {
public:
    virtual bool charge(const std::string& cardNumber, double amount) = 0;
    virtual ~IPaymentGateway() = default;
};
```

```cpp
// RealPaymentGateway.h
#pragma once
#include "IPaymentGateway.h"
#include <iostream>

class RealPaymentGateway : public IPaymentGateway {
public:
    bool charge(const std::string& cardNumber, double amount) override {
        std::cout << "[Stripe] Изпращане на транзакция за " << amount << " лв.\n";
        return true; // реална HTTP заявка
    }
};
```

```cpp
// MockPaymentGateway.h - за тестове
#pragma once
#include "IPaymentGateway.h"

class MockPaymentGateway : public IPaymentGateway {
    bool shouldSucceed;
    int callCount = 0;
    double lastAmount = 0.0;
    std::string lastCard;

public:
    explicit MockPaymentGateway(bool shouldSucceed = true)
        : shouldSucceed(shouldSucceed) {}

    bool charge(const std::string& cardNumber, double amount) override {
        callCount++;
        lastAmount = amount;
        lastCard = cardNumber;
        return shouldSucceed; // контролираме резултата в теста
    }

    // Методи за проверка (verify)
    int getCallCount() const { return callCount; }
    double getLastAmount() const { return lastAmount; }
    const std::string& getLastCard() const { return lastCard; }
};
```

---

### OrderProcessor с Dependency Injection

```cpp
// OrderProcessor.h - зависи от интерфейс, не от конкретна реализация
#pragma once
#include "IPaymentGateway.h"
#include <string>
#include <stdexcept>

class OrderProcessor {
    IPaymentGateway& gateway; // ← зависимост чрез интерфейс

public:
    explicit OrderProcessor(IPaymentGateway& gateway) : gateway(gateway) {}

    bool processOrder(const std::string& cardNumber, double amount) {
        if (amount <= 0) throw std::invalid_argument("Сумата трябва да е положителна");
        if (cardNumber.empty()) throw std::invalid_argument("Картата не може да е празна");
        return gateway.charge(cardNumber, amount);
    }
};
```

---

### Тестове с Mock

```cpp
#include "OrderProcessor.h"
#include "MockPaymentGateway.h"
#include <iostream>
#include <cassert>

void test_successful_payment_is_processed() {
    // Arrange
    MockPaymentGateway mockGateway(true); // симулираме успех
    OrderProcessor processor(mockGateway);

    // Act
    bool result = processor.processOrder("4111111111111111", 150.0);

    // Assert - резултат
    assert(result == true);
    // Assert - взаимодействие с gateway
    assert(mockGateway.getCallCount() == 1);
    assert(mockGateway.getLastAmount() == 150.0);
    std::cout << "[PASS] успешно плащане е обработено\n";
}

void test_failed_payment_returns_false() {
    // Arrange - симулираме провал на gateway (напр. изтекла карта)
    MockPaymentGateway mockGateway(false);
    OrderProcessor processor(mockGateway);

    // Act
    bool result = processor.processOrder("4111111111111111", 150.0);

    // Assert
    assert(result == false);
    assert(mockGateway.getCallCount() == 1);
    std::cout << "[PASS] провалено плащане връща false\n";
}

void test_gateway_not_called_for_invalid_order() {
    // Arrange
    MockPaymentGateway mockGateway;
    OrderProcessor processor(mockGateway);

    // Act & Assert
    try {
        processor.processOrder("4111111111111111", -50.0); // невалидна сума
        assert(false);
    } catch (const std::invalid_argument&) {
        // gateway не трябва да е извикан за невалидна поръчка
        assert(mockGateway.getCallCount() == 0);
        std::cout << "[PASS] gateway не е извикан при невалидна поръчка\n";
    }
}

int main() {
    test_successful_payment_is_processed();
    test_failed_payment_returns_false();
    test_gateway_not_called_for_invalid_order();
    std::cout << "\nВсички mock тестове преминаха!\n";
    return 0;
}
```

---

### Видове тест дублъори

| Тип      | Описание                                               | Пример                                  |
| -------- | ------------------------------------------------------ | --------------------------------------- |
| **Stub** | Връща фиксиран отговор, не проверява извиквания        | `MockGateway(true)` - винаги успешен    |
| **Mock** | Проверява **как** е бил извикан (брой, параметри)      | `assert(callCount == 1)`                |
| **Spy**  | Реален обект, но записва извикванията                  | Реален Logger, който помним за тестовете |
| **Fake** | Опростена, но работеща реализация                      | In-memory база данни вместо реална       |

---

### Mocking и ООП принципите

```
Dependency Inversion Principle:
  Зависи от абстракции (интерфейси), не от конкретни класове.

           ┌────────────────────┐
           │   OrderProcessor   │
           └────────┬───────────┘
                    │ зависи от
                    ▼
           ┌────────────────────┐
           │  IPaymentGateway   │  ← интерфейс
           └────────┬───────────┘
                    │
           ┌────────┴────────┐
           ▼                 ▼
   RealPaymentGateway   MockPaymentGateway
   (в продукция)        (в тестове)
```

> **Правило**: Ако кодът ти е лесно мокаем, той следва добрите ООП принципи.

---

## Обобщение

**Тестване** = проверка, че кодът работи сега И след промени

**Четири нива:**

| Ниво            | Тества                                         |
| --------------- | ---------------------------------------------- |
| **Unit**        | Един клас/метод изолирано (бързо, много тестове) |
| **Integration** | Взаимодействие между компоненти                |
| **System**      | Цялата система като едно цяло (реални зависимости) |
| **Acceptance**  | Цял потребителски сценарий (Given/When/Then)   |

**TDD цикъл:**

| Фаза           | Действие                                        |
| -------------- | ----------------------------------------------- |
| **RED**        | Пиши тест, който пропада                        |
| **GREEN**      | Пиши минималния код, който кара теста да мине   |
| **REFACTOR**   | Подобри кода (тестовете трябва да минат)        |

**Mocking:**

| Вид      | Описание                               |
| -------- | -------------------------------------- |
| **Stub** | Връща фиксиран отговор                 |
| **Mock** | Проверява как е бил извикан            |
| **Spy**  | Записва реални извиквания              |
| **Fake** | Опростена, но работеща реализация      |

**Принципи:**

| Принцип                           | Значение                                    |
| --------------------------------- | ------------------------------------------- |
| **AAA**                           | Arrange → Act → Assert                      |
| **Един тест = едно нещо**         | Не тествай повече от едно поведение наведнъж |
| **Трудно тестваем код**           | Индикатор за лош дизайн                     |
| **Лесно мокаем код**              | Следва DIP и добрите ООП принципи           |
