#include <exception>
#include <string>
#include <regex>
using namespace std;

class ValidationError : public exception {
    string msg;
public:
    explicit ValidationError(string m) : msg(move(m)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

// Разрешаваме букви (латиница) и тире. (За по-широка азбука – адаптирайте регекса/локала.)
void validateName(const string& name) {
    if (name.size() < 2 || name.size() > 30)
        throw ValidationError("Дължината трябва да е 2–30 символа");
    static const regex rx("^[A-Za-z-]+$");  // само букви A–Z/a–z и тире
    if (!regex_match(name, rx))
        throw ValidationError("Позволени са само букви и тире");
}
