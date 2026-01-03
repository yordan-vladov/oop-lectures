# ООП - Практическо контролно примерни въпроси
## Вариант 1: Наследяване и виртуални методи

1. Създайте базов клас `Employee` със следните член-данни:
    * `string name` – име
    * `double baseSalary` – основна заплата

2. В класа `Employee` да има:
    * конструктор с параметри
    * метод `virtual double monthlyPay() const`, който **не е чисто виртуален** и по подразбиране връща `baseSalary`
    * метод `virtual void print() const`, който извежда:
     `Employee: <име>, base salary: <baseSalary>, monthly pay: <monthlyPay()>`

3. Създайте наследник `SalesEmployee`, който добавя:
    * `double bonus` – бонус

4. В `SalesEmployee`:
    * предефинирайте (`override`) `monthlyPay()` така, че да връща `baseSalary + bonus`
    * предефинирайте `print()` така, че да извежда:
     `SalesEmployee: <име>, base: <baseSalary>, bonus: <bonus>, monthly pay: <monthlyPay()>`

5. В `main()` метод:
    * създайте обект `Employee` и обект `SalesEmployee`
    * създайте указател `Employee* p`, който последователно да сочи към двата обекта
    * извикайте `p->print()` и за двата случая, за да се демонстрира полиморфизъм

## Вариант 2: Абстракция и интерфейси

1. Създайте интерфейс `IPrintable` (абстрактен клас), който съдържа:
    * `virtual void printText(const string& text) = 0;`
    * виртуален деструктор

2. Създайте интерфейс `IScannable`, който съдържа:
    * `virtual string scan() = 0;`
    * виртуален деструктор

3. Създайте клас `MultifunctionDevice`, който **имплементира и двата интерфейса** и има член-данна:
    * `string model` – модел на устройството

4. Реализирайте методите:
    * `printText(text)` извежда на екрана:
     `"<model> prints: <text>"`
    * `scan()` връща текст:
     `"Scanned document by <model>"`

5. В `main()` метод:
    * създайте обект `MultifunctionDevice`
    * използвайте го чрез:
    * указател/референция от тип `IPrintable*` и извикайте `printText(...)`
    * указател/референция от тип `IScannable*` и извикайте `scan()` и изведете резултата