# Model-View-Controller

---

## Съдържание

1. Какво е MVC?
2. Проблемите, които MVC решава
3. Структура на шаблона
4. Прост пример на C++
5. Model-View-ViewModel (MVVM)
6. MVC vs MVVM — сравнение

---

## 1. Какво е MVC?

**Model-View-Controller** е архитектурен шаблон за проектиране, който разделя логиката на приложението на **три независими компонента**.

---

![](https://developer.mozilla.org/en-US/docs/Glossary/MVC/model-view-controller-light-blue.png)

---

| Компонент      | Отговорност                              |
| -------------- | ---------------------------------------- |
| **Model**      | Данните и бизнес логиката                |
| **View**       | Визуализация на данните за потребителя   |
| **Controller** | Обработва входа, координира Model и View |

> **Принцип**: Всеки компонент не знае за вътрешната реализация на другите.

---

## 2. Проблемите, които MVC решава

### Без MVC — "Spaghetti code"

```cpp
// БЕЗ MVC — всичко е в едно
class StudentApp {
 vector<string> students;

 void run() {
 // Данни, UI и логика — смесени
 cout << "Въведи студент: ";
 string name;
 cin >> name;
 students.push_back(name); // данни
 cout << "Добавен: " << name << endl; // UI
 // Логика, валидация, ... всичко тук
 }
};
```

---

### Проблеми:

- **Трудно тестване** — не можеш да тестваш логиката без UI
- **Дублиране** — промяна в UI налага промяна в логиката
- **Мащабиране** — при нов View трябва да се пренапише всичко
- **Екипна работа** — UI и backend разработчиците си пречат

---

### С MVC — разделение на отговорностите

- Можеш да смениш UI без да пипаш данните
- Лесно unit тестване на Model
- Множество View-та за един Model (GUI + CLI + Web)
- Паралелна работа на различни части

---

## 3. Структура на шаблона

---

### Model

- Съдържа данните (state)
- Съдържа бизнес правилата
- НЕ знае нищо за View
- Уведомява наблюдателите при промяна (Observer pattern)

---

### View

- Визуализира данните от Model
- НЕ съдържа бизнес логика
- Получава данни — показва ги
- Препраща потребителски действия към Controller

---

### Controller

- Посредник между Model и View
- Обработва входа от потребителя
- Взима решения: какво да се промени в Model
- Избира кой View да се покаже

---

## 4. Прост пример на C++

**Сценарий**: Приложение за управление на студенти

---

### Model — `Student.h`

```cpp
#pragma once
#include <string>
#include <vector>
#include <functional>

class StudentModel {
private:
 std::vector<std::string> students;
 std::vector<std::function<void()>> observers; // Observer pattern

public:
 // Регистрира наблюдател (View)
 void addObserver(std::function<void()> observer) {
 observers.push_back(observer);
 }

 // Уведомява всички наблюдатели при промяна
 void notifyObservers() {
 for (auto& obs : observers) obs();
 }

 void addStudent(const std::string& name) {
 if (!name.empty()) {
 students.push_back(name);
 notifyObservers(); // ← Model уведомява View
 }
 }

 void removeStudent(int index) {
 if (index >= 0 && index < (int)students.size()) {
 students.erase(students.begin() + index);
 notifyObservers();
 }
 }

 const std::vector<std::string>& getStudents() const {
 return students;
 }

 int getCount() const {
 return students.size();
 }
};
```

---

### View — `StudentView.h`

```cpp
#pragma once
#include <iostream>
#include <string>
#include <vector>

class StudentView {
public:
 // Показва списъка с студенти
 void displayStudents(const std::vector<std::string>& students) {
 std::cout << "\n===== Списък на студенти =====\n";
 if (students.empty()) {
 std::cout << " (няма добавени студенти)\n";
 } else {
 for (int i = 0; i < students.size(); i++) {
 std::cout << " [" << i << "] " << students[i] << "\n";
 }
 }
 std::cout << " Общо: " << students.size() << " студента\n";
 std::cout << "==================================\n";
 }

 // Показва меню
 void displayMenu() {
 std::cout << "\n[1] Добави студент\n";
 std::cout << "[2] Премахни студент\n";
 std::cout << "[0] Изход\n";
 std::cout << "Избор: ";
 }

 // Взима вход от потребителя
 std::string getStudentName() {
 std::cout << "Въведи имe: ";
 std::string name;
 std::cin >> name;
 return name;
 }

 int getChoice() {
 int choice;
 std::cin >> choice;
 return choice;
 }

 int getIndex() {
 std::cout << "Въведи индекс: ";
 int idx;
 std::cin >> idx;
 return idx;
 }

 void showMessage(const std::string& msg) {
 std::cout << "→ " << msg << "\n";
 }
};
```

---

### Controller — `StudentController.h`

```cpp
#pragma once
#include "StudentModel.h"
#include "StudentView.h"

class StudentController {
private:
 StudentModel& model;
 StudentView& view;

public:
 StudentController(StudentModel& m, StudentView& v)
 : model(m), view(v)
 {
 // Controller свързва Model и View чрез Observer
 model.addObserver([this]() {
 this->view.displayStudents(this->model.getStudents());
 });
 }

 void addStudent() {
 std::string name = view.getStudentName();
 model.addStudent(name);
 view.showMessage("Студентът е добавен успешно.");
 }

 void removeStudent() {
 int idx = view.getIndex();
 model.removeStudent(idx);
 view.showMessage("Студентът е премахнат.");
 }

 void run() {
 // Начален дисплей
 view.displayStudents(model.getStudents());

 bool running = true;
 while (running) {
 view.displayMenu();
 int choice = view.getChoice();

 switch (choice) {
 case 1: addStudent(); break;
 case 2: removeStudent(); break;
 case 0: running = false; break;
 default:
 view.showMessage("Невалиден избор.");
 }
 }
 }
};
```

---

### `main.cpp`

```cpp
#include "StudentModel.h"
#include "StudentView.h"
#include "StudentController.h"

int main() {
 StudentModel model;
 StudentView view;
 StudentController controller(model, view);

 controller.run();

 return 0;
}
```

---

### Изходен резултат

```
===== Списък на студенти =====
 (няма добавени студенти)
 Общо: 0 студента
==================================

[1] Добави студент
[2] Премахни студент
[0] Изход
Избор: 1
Въведи имe: Иван

===== Списък на студенти =====
 [0] Иван
 Общо: 1 студента
==================================
→ Студентът е добавен успешно.
```

---

## 5. Model-View-ViewModel (MVVM)

MVVM е еволюция на MVC, въведена от Microsoft за UI frameworks (WPF, Angular, Qt).

---

### Разлика: ViewModel вместо Controller

![](https://upload.wikimedia.org/wikipedia/commons/thumb/d/d5/MVVMPattern.svg/960px-MVVMPattern.svg.png)

---

### Ключова идея — **Data Binding**

Вместо View да извиква Controller, View се **"свързва"** директно с данните на ViewModel. Промяна в ViewModel → автоматично се обновява View.

---

### MVVM пример на C++

```cpp
#include <string>
#include <functional>
#include <iostream>

// Model
struct Student {
 std::string name;
 int grade;
};

// ViewModel
class StudentViewModel {
private:
 Student data;

 // Callback за двупосочен binding (при реален framework — автоматично)
 std::function<void(const std::string&, int)> onChanged;

public:
 void setOnChanged(std::function<void(const std::string&, int)> cb) {
 onChanged = cb;
 }

 // Команда — капсулира действие (Command pattern)
 void updateGrade(int newGrade) {
 if (newGrade >= 2 && newGrade <= 6) {
 data.grade = newGrade;
 if (onChanged) onChanged(data.name, data.grade); // уведомява View
 }
 }

 void setName(const std::string& name) {
 data.name = name;
 if (onChanged) onChanged(data.name, data.grade);
 }

 // Подготвени данни за View (не raw данни от Model!)
 std::string getDisplayName() const { return "Студент: " + data.name; }
 std::string getDisplayGrade() const { return "Оценка: " + std::to_string(data.grade); }
};

// View
class StudentViewMVVM {
public:
 // "Binding" — при промяна в ViewModel, View се обновява автоматично
 void bind(StudentViewModel& vm) {
 vm.setOnChanged([this](const std::string& name, int grade) {
 this->render(name, grade);
 });
 }

 void render(const std::string& name, int grade) {
 std::cout << "[View обновен] " << name
 << " | Оценка: " << grade << "\n";
 }
};

// main
int main() {
 StudentViewModel vm;
 StudentViewMVVM view;

 view.bind(vm); // Свързваме View с ViewModel

 vm.setName("Мария"); // → View се обновява автоматично
 vm.updateGrade(5); // → View се обновява автоматично
 vm.updateGrade(6); // → View се обновява автоматично

 return 0;
}
```

---

**Изход:**

```
[View обновен] Мария | Оценка: 0
[View обновен] Мария | Оценка: 5
[View обновен] Мария | Оценка: 6
```

---

## 6. MVC vs MVVM — Сравнение

| Критерий              | MVC                            | MVVM                                |
| --------------------- | ------------------------------ | ----------------------------------- |
| **Посредник**         | Controller                     | ViewModel                           |
| **Комуникация**       | Controller дърпа данните       | Двупосочен binding                  |
| **View зависи от...** | Controller                     | ViewModel (не Controller)           |
| **Тестване**          | Controller трябва mock на View | ViewModel е чист C++ — лесно        |
| **Подходящ за**       | Backend, Desktop (Qt/wx)       | UI-heavy apps, React/Angular/Qt QML |
| **Сложност**          | По-просто                      | По-сложно (binding механизъм)       |

---

### Кога да избереш кое?

```
MVC → бизнес приложения, REST API backends, прости десктоп програми
MVVM → React, Angular, Qt QML, WPF — където UI binding е нативен
```

---

## Резюме

```
 MVC = Разделение на отговорностите

 Model → "Какво знам" (данни + правила)
 View → "Как изглеждам" (визуализация)
 Controller → "Какво правя" (логика на входа)

 MVVM = MVC + автоматичен data binding
 ViewModel замества Controller с реактивен state
```
