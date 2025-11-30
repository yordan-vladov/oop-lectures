
# Домашна работа: Мини „Библиотечна система“

## Цел

Целта на задачата е да се проектира и имплементира малка система за библиотека, съставена от няколко взаимосвързани класа, която да демонстрира прилагането на добри обектно-ориентирани практики в C++, включително капсулация, работа с конструктори и деструктори, използване на статични членове, константни методи, валидация на данни и прилагане на принципите на „Rule of 3/5“.
## Изисквания към класовете

### 1) `Author`

* **Членове**: `string name`, `int birthYear`.
* **Конструктори**:
  * по подразбиране (`name = "Unknown"`, `birthYear = 1900`);
  * параметризиран (използвайте *списък за инициализация*).
* **Методи**:
  * `string to_string() const;`
  * getters (и `setBirthYear(int)` с валидиране: разумен диапазон, напр. 1850–2025).
* **Добри практики**: `const`-коректност, `explicit` при едноаргументни конструктори (ако добавите такъв).

### 2) `Book`

* **Членове**: `string title`, `Author author`, `int year`, `double price`, `string isbn`.
* **Статични членове**: `static int totalBooks;` (брои живите инстанции).
* **Конструктори**:
  * по подразбиране;
  * параметризиран (използвайте *списък за инициализация*);
  * **копиращ** и **преместващ** конструктор (виж Rule of 5).
  
* **Оператори (Rule of 5)**:
  * копиращ оператор `operator=`;
  * преместващ оператор `operator=`;
  * **деструктор** (ако нямате ресурс – оставете `= default`, но все пак демонстрирайте Rule of 5).
  * Забележка: ако нямате собствен ресурс, може да маркирате копиране/преместване като `= default`. Целта е да покажете, че знаете *кога* да дефинирате/дефолтнете.

* **Методи**:
  * валидиране в setters (например цена ≥ 0; `year` разумен);
  * `string to_string() const;`
  * `static int getTotalBooks();`
* **Добри практики**: `explicit` при едноаргументен конструктор (ако имате), `const` методи, `= default`/`= delete` където е уместно.

### 3) `Member` (читател)

* **Членове**: `string name`, `string memberId`, `int yearJoined`.
* **Методи**:
  * валидации (например `memberId` да не е празен);
  * `string to_string() const;`
* **Конструктори**: по подразбиране и параметризиран.

### 4) `Loan` (заем)

* Представя заемане на книга от читател с дати.
* **Членове**: `string isbn`, `string memberId`, `string startDate`, `string dueDate`, `bool returned`.
* **Методи**:
  * `void markReturned();`
  * `bool isOverdue(const string& today) const;` *(проста лексикографска проверка YYYY-MM-DD е достатъчна за целта на задачата)*
  * `string to_string() const;`
* **Конструктор**: параметризиран (валидирайте, че `dueDate >= startDate`).

### 5) `Library`

* **Членове**: `vector<Book> books;`, `vector<Member> members;`, `vector<Loan> loans;`
* **Методи** (минимум):
  * `void addBook(const Book& b);`
  * `void addMember(const Member& m);`
  * `bool hasBook(const string& isbn) const;`
  * `bool isBookAvailable(const string& isbn) const;` *(няма активен неприключен заем)*
  * `bool loanBook(const string& isbn, const string& memberId, const string& start, const string& due);`
  * `bool returnBook(const string& isbn, const string& memberId);`
  * `vector<Book> findByAuthor(const string& authorName) const;`
  * `string to_string() const;` *(обобщена информация – брой книги/членове/активни заеми)*
* **Добри практики**: максимално `const`-коректност, без „голи“ указатели, ясни инварианти.
---

## Пример за `main()`

```cpp
#include <iostream>
#include "Library.h"
using namespace std;

int main() {
    Library lib;
    Author a{"Ivan Vazov", 1850};
    Book b1{"Pod igoto", a, 1894, 25.50, "ISBN-001"};
    Book b2{"Nema zemya", a, 1900, 18.90, "ISBN-002"};

    lib.addBook(b1);
    lib.addBook(b2);
    lib.addMember(Member{"Petar Petrov", "M001", 2023});

    cout << lib.to_string() << "\n";

    if (lib.loanBook("ISBN-001", "M001", "2025-11-03", "2025-11-17"))
        cout << "Loan created.\n";

    cout << "Available ISBN-001? " << boolalpha << lib.isBookAvailable("ISBN-001") << "\n";
    lib.returnBook("ISBN-001", "M001");
    cout << "Available ISBN-001? " << boolalpha << lib.isBookAvailable("ISBN-001") << "\n";

    for (const auto& bk : lib.findByAuthor("Vazov")) {
        cout << bk.to_string() << "\n";
    }
}
```

---
## Критерии за оценяване

* **Коректност и устойчивост** (валидации, инварианти): 30%
* **ООП дизайн** (капсулация, `const`-коректност, статични членове): 25%
* **Конструктори/деструктори, Rule of 3/5**: 20%
* **Код стил и добри практики** (`explicit`, списъци за инициализация, `= default`/`= delete`): 15%
* **Демонстрация и тестове** (смислен `main()`, покриващ случаи): 10%

---
## Какво да предадете

- Линк към GitHub хранилище, което да включва:
    * Изходен код (`.h`/`.cpp`) 
    * `README.md`, което да включва:
        * как се компилира (напр. `g++ -std=gnu++17 -Wall -Wextra -O2 -o library main.cpp library.cpp`),
        * кратко описание на класовете.
        * Примерен изход от програмата.
