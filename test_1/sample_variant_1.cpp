#include <iostream>
#include <string>
using namespace std;

class Employee {
protected:
    string name;
    double baseSalary;

public:
    Employee(const string& name, double baseSalary)
        : name(name), baseSalary(baseSalary) {}

    // НЕ е чисто виртуален
    virtual double monthlyPay() const {
        return baseSalary;
    }

    virtual void print() const {
        cout << "Employee: " << name
             << ", base salary: " << baseSalary
             << ", monthly pay: " << monthlyPay()
             << endl;
    }

    virtual ~Employee() {}
};

class SalesEmployee : public Employee {
private:
    double bonus;

public:
    SalesEmployee(const string& name, double baseSalary, double bonus)
        : Employee(name, baseSalary), bonus(bonus) {}

    double monthlyPay() const override {
        return baseSalary + bonus;
    }

    void print() const override {
        cout << "SalesEmployee: " << name
             << ", base: " << baseSalary
             << ", bonus: " << bonus
             << ", monthly pay: " << monthlyPay()
             << endl;
    }
};

int main() {
    Employee e("Ivan", 1200);
    SalesEmployee s("Maria", 1200, 300);

    Employee* p = nullptr;

    p = &e;
    p->print();

    p = &s;
    p->print();

    return 0;
}
