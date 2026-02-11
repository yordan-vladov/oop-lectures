# Singleton

---

## Съдържание

* Какво представлява
* Как се реализира
* Кога се използва
* Проблеми и ограничения

---

## Припомняне: Класове и статични членове

* Какво е клас?
* Какво означава `static` член?
* Каква е разликата между обект и клас?
* Кога използваме private конструктор?

---

## Проблемът

Представете си система, в която:

* Имаме логване във файл
* Имаме връзка към база данни
* Имаме глобална конфигурация

---

## Какво е Singleton?

Singleton е шаблон за проектиране, който:

* Позволява съществуването само на една инстанция
* Осигурява глобален достъп до нея
* Контролира създаването на обекта

---

## Основна идея

За да реализираме Singleton:

* Конструкторът е private
* Забраняваме копиране
* Имаме static метод `getInstance()`
* Имаме static променлива за инстанцията

---

## Класическа реализация (lazy initialization)

```cpp
class Singleton {
private:
    static Singleton* instance;
    Singleton() {}

public:
    static Singleton* getInstance() {
        if (instance == nullptr)
            instance = new Singleton();
        return instance;
    }
};
```

---

## Проблеми при тази реализация

* Memory leak (няма delete)
* Не е thread-safe
* Сложност при управление на ресурси

---

## Eager Initialization

```cpp
class Singleton {
private:
    static Singleton instance;
    Singleton() {}

public:
    static Singleton& getInstance() {
        return instance;
    }
};

Singleton Singleton::instance;
```
---

- **Проблеми:**
    * Обектът се създава винаги 
    * Проблеми с реда на инициализация
    * Липса на lazy loading

---

## Meyers Singleton (C++11+)

```cpp
class Singleton {
private:
    Singleton() {}

public:
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }
};
```
---

- **Предимства:**
    * Lazy initialization
    * Thread-safe
    * Няма memory leak
    * Най-добра практика
---

# Примери

---

## Logger

**Проблем:** Не искаме всички логове да пишат в един файл.

```cpp
class Logger {
private:
    ofstream file;
    Logger() { file.open("log.txt"); }

public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const string& msg) {
        file << msg << endl;
    }
};
```

---

## Връзка към база данни

**Проблем:** Създаването на множество връзки към БД е скъпо.

```cpp
class DatabaseConnection {
private:
    DatabaseConnection() {}

public:
    static DatabaseConnection& getInstance() {
        static DatabaseConnection instance;
        return instance;
    }

    void query(string sql) {
        cout << sql << endl;
    }
};
```

---

## Конфигурационен клас

**Проблем:** Конфигурацията трябва да е една и съща навсякъде.

```cpp
class Config {
private:
    map<string,string> settings;
    Config() { settings["mode"] = "debug"; }

public:
    static Config& getInstance() {
        static Config instance;
        return instance;
    }

    string get(string key) {
        return settings[key];
    }
};
```

---

## Недостатъци

* Скрит глобален обект
* Трудно тестване
* Нарушава SRP
* Създава силна зависимост

---

## Кога е подходящ?

Подходящ:

* Логически има само една инстанция
* Управлява глобален ресурс

---

Неподходящ:

* За удобство
* Като универсално решение

---

## Практическа задача

Създайте клас `ApplicationState`, който:

* Е Singleton
* Съхранява текущ потребител (username)
* Има методи `setUser()` и `getUser()`
