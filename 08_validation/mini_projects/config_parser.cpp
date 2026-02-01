#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <cctype>
using namespace std;

string trim(const string& s) {
    size_t i = 0, j = s.size();
    while (i < j && isspace((unsigned char)s[i])) ++i;
    while (j > i && isspace((unsigned char)s[j-1])) --j;
    return s.substr(i, j - i);
}

bool isInteger(const string& v) {
    if (v.empty()) return false;
    size_t i = 0;
    if (v[0] == '+' || v[0] == '-') i = 1;
    if (i == v.size()) return false;
    for (; i < v.size(); ++i) if (!isdigit((unsigned char)v[i])) return false;
    return true;
}

struct ConfigValue {
    enum Type { INT, BOOL, STRING } type;
    long long i = 0;
    bool b = false;
    string s;
};

unordered_map<string, ConfigValue> parseConfig(istream& in) {
    unordered_map<string, ConfigValue> cfg;
    string line;
    int lineno = 0;

    while (getline(in, line)) {
        ++lineno;
        string t = trim(line);
        if (t.empty() || t[0] == '#') continue;

        size_t eq = t.find('=');
        if (eq == string::npos) {
            throw invalid_argument("Липсва '=' на ред " + to_string(lineno));
        }

        string key = trim(t.substr(0, eq));
        string val = trim(t.substr(eq + 1));
        if (key.empty()) throw invalid_argument("Празен ключ на ред " + to_string(lineno));

        ConfigValue cv;
        if (val == "true" || val == "false") {
            cv.type = ConfigValue::BOOL;
            cv.b = (val == "true");
        } else if (isInteger(val)) {
            cv.type = ConfigValue::INT;
            cv.i = stoll(val);
        } else {
            cv.type = ConfigValue::STRING;
            cv.s = val;
        }
        cfg[key] = cv;
    }
    return cfg;
}

int main() {
    // Демонстрация с примерен вход
    istringstream sample(R"(# пример
port=8080
debug=true
name=MyApp
timeoutMs=1500
)");

    try {
        auto cfg = parseConfig(sample);
        for (auto& [k, v] : cfg) {
            cout << k << " = ";
            if (v.type == ConfigValue::INT) cout << v.i << " (int)\n";
            else if (v.type == ConfigValue::BOOL) cout << (v.b ? "true" : "false") << " (bool)\n";
            else cout << v.s << " (string)\n";
        }
    } catch (const exception& e) {
        cerr << "Грешка: " << e.what() << "\n";
    }
}
