#include <iostream>
#include <string>
using namespace std;

class IPrintable {
public:
    virtual void printText(const string& text) = 0;
    virtual ~IPrintable() {}
};

class IScannable {
public:
    virtual string scan() = 0;
    virtual ~IScannable() {}
};

class MultifunctionDevice : public IPrintable, public IScannable {
private:
    string model;

public:
    MultifunctionDevice(const string& model) : model(model) {}

    void printText(const string& text) override {
        cout << model << " prints: " << text << endl;
    }

    string scan() override {
        return "Scanned document by " + model;
    }
};

int main() {
    MultifunctionDevice dev("HP-Laser-2000");

    IPrintable* p = &dev;
    p->printText("Hello OOP!");

    IScannable* s = &dev;
    cout << s->scan() << endl;

    return 0;
}
