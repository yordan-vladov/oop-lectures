# Организация на C++ кода: файлове, проекти, CMake

---

## Защо е важна организацията на кода?

* По-лесно **четене и поддръжка** на проекта.
* Позволява **повторно използване** на код (библиотеки).
* Подобрява **времето за компилация** и структурата на проекта.
* Прави преминаването към по-големи проекти и екипна работа много по-лесно.

---
## Header файлове (`.h / .hpp`)

* **Header (.h / .hpp)**:
  * съдържат **декларации**:
    * на функции;
    * на класове, структури, enum-и;
    * константи.
  * не трябва да съдържат "тежка" логика, освен `inline` функции/шаблони.
---
## Изходни файлове (`.cpp`)

* **Source (.cpp)**:
  * съдържат **имплементациите** (дефинициите) на:
    * методите на класовете;
    * свободните функции;
    * логиката на програмата.
---

```cpp
// String.h
#pragma once

class String {
public:
    String(const char* s);
    ~String();

    const char* c_str() const;

private:
    char* data;
    size_t size;
};
```
---
```cpp
// String.cpp
#include "String.h"
#include <cstring>

String::String(const char* s) {
    size = strlen(s);
    data = new char[size + 1];
    strcpy(data, s);
}

String::~String() {
    delete[] data;
}

const char* String::c_str() const {
    return data;
}
```

---

## Include guards и `#pragma once`

* Предотвратяват **многократно включване** на един и същ header (multiple inclusion).
* Вариант 1 – include guards:

```cpp
#ifndef STRING_H
#define STRING_H

// съдържанието на String.h

#endif // STRING_H
```
---
* Вариант 2 – `#pragma once` (поддържа се от повечето компилатори):

```cpp
#pragma once
// съдържанието на header-а
```

---

## Примерна структура на малък C++ проект

```text
project/
├─ include/
│  ├─ String.h
│  ├─ DynamicArray.h
├─ src/
│  ├─ String.cpp
│  ├─ DynamicArray.cpp
│  ├─ main.cpp
├─ CMakeLists.txt
```
---
* `include/` – header файлове, достъпни за целия проект (и за други проекти).
* `src/` – имплементации и `main.cpp`.
* `CMakeLists.txt` – конфигурация за CMake (build система).

---

## Библиотеки: статични и динамични

* **Статична библиотека** (`.lib`, `.a`):
  * кодът се **включва** в изпълнимия файл при линкване.
  * по-голям `.exe`, но по-лесно разпространение.
---
* **Динамична библиотека** (`.dll`, `.so`):
  * зарежда се по време на изпълнение.
  * по-малък `.exe`, но трябва да носите и `.dll`/`.so` файловете.
* Идея:
  * Общи класове/модули → в библиотека.
  * Програмите → линкват тази библиотека.

---

## Какво е CMake?

* **CMake** – генератор на build системи:
  * не компилира директно;
  * генерира проект за:
    * Makefiles;
    * Visual Studio;
    * Ninja и др.
---
* Позволява:
  * лесно добавяне на нови файлове/библиотеки;
  * преносимост между различни платформи и IDE-та.

---

## Прост пример за `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

# Добавяме изпълним файл от .cpp файловете
add_executable(MyProject
    src/main.cpp
    src/String.cpp
    src/DynamicArray.cpp
)

# Добавяме директорията с header-и
target_include_directories(MyProject PUBLIC include)
```
---
* Команди:

  * `add_executable` – създава изпълним файл.
  * `target_include_directories` – казва къде са header-ите.

---

## Добавяне на библиотека с CMake

```cmake
# Статична библиотека от тези файлове
add_library(MyLib STATIC
    src/String.cpp
    src/DynamicArray.cpp
)

target_include_directories(MyLib PUBLIC include)

# Изпълнима програма, която използва библиотеката
add_executable(MyApp src/main.cpp)

# Линкваме библиотеката към приложението
target_link_libraries(MyApp PRIVATE MyLib)
```

* Така `MyLib` може да бъде използвана и от други проекти.

---

## Добри практики за организация на кода

* Един клас / модул → **в отделни `.h` и `.cpp` файлове**.
* Използвайте **пространства от имена** (`namespace`) за избягване на конфликти:

```cpp
namespace utils {
    void printLine(const char* msg);
}
```

* Не слагайте `using namespace std;` в header файлове.
* Дръжте **интерфейса** (декларации в `.h`) чист и ясен.
* Коментирайте *публичните* методи – какво правят, не как.

---

## Обобщение: организацията на кода в C++

* Разделяйте декларации (в header-и) и дефиниции (в `.cpp`).
* Използвайте **include guards** или `#pragma once`.
* Организирайте проекта в директории: `include/`, `src/`, `lib/` и др.
* Използвайте **библиотеки** за общи модули.
---
* Използвайте **CMake** (или друга build система) вместо ръчно компилиране.
* Добрата структура прави:
  * проектите по-лесни за **разширяване**;
  * кода по-лесен за **разбиране** и **повторна употреба**.
---
### 4. **Задача: Организация на проект с CMake**

* Създайте C++ проект със следната структура:

```text
project/
├─ include/
│  ├─ MathUtils.h
├─ src/
│  ├─ MathUtils.cpp
│  ├─ main.cpp
├─ CMakeLists.txt
```
---
* В `MathUtils` дефинирайте клас с няколко статични метода (например: сума, средно, максимална стойност).
* Използвайте **CMake**, за да:
  * компилирате проекта;
  * линкнете библиотека (`MathUtils`) към изпълнимия файл (`main`).
* Добавете поне две компилационни единици (cpp файлове), за да упражните разделянето на интерфейс и имплементация.