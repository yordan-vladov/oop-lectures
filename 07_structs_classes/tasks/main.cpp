#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstddef>

using namespace std;

/*==================== 1) Date + isLeap ====================*/
struct Date {
    int d, m, y;
};

// Високосна ли е годината на дадена дата?
bool isLeap(const Date& dt) {
    int y = dt.y;
    return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

/*==================== 2) Student + normalize/print ====================*/
struct Student {
    string name;
    int fn;
    double grade;
};

void normalize(Student& s) {
    if (s.grade < 2.0) s.grade = 2.0;
    if (s.grade > 6.0) s.grade = 6.0;
}

void print(const Student& s) {
    cout << "Student{name=\"" << s.name
         << "\", fn=" << s.fn
         << ", grade=" << s.grade << "}\n";
}

/*==================== 3) Клас Vector<double> (Rule of 5) ====================*/
class Vector {
public:
    // конструктор с размер
    explicit Vector(size_t n)
        : m_size(n), m_data(n ? new double[n]() : nullptr) {}

    // деструктор
    ~Vector() { delete[] m_data; }

    // копиращ конструктор
    Vector(const Vector& other)
        : m_size(other.m_size), m_data(other.m_size ? new double[other.m_size] : nullptr) {
        std::copy(other.m_data, other.m_data + m_size, m_data);
    }

    // премествaщ конструктор
    Vector(Vector&& other) noexcept
        : m_size(other.m_size), m_data(other.m_data) {
        other.m_size = 0;
        other.m_data = nullptr;
    }

    // копиращо присвояване
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector tmp(other);          // copy-and-swap идиом
            swap(tmp);
        }
        return *this;
    }

    // премествaщо присвояване
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] m_data;
            m_size = other.m_size;
            m_data = other.m_data;
            other.m_size = 0;
            other.m_data = nullptr;
        }
        return *this;
    }

    // оператор [] (non-const)
    double& operator[](size_t i) {
        return m_data[i]; // без проверка за граници за скорост
    }

    // оператор [] (const)
    const double& operator[](size_t i) const {
        return m_data[i];
    }

    size_t size() const { return m_size; }

    // помощен swap
    void swap(Vector& other) noexcept {
        std::swap(m_size, other.m_size);
        std::swap(m_data, other.m_data);
    }

private:
    size_t m_size{0};
    double* m_data{nullptr};
};

/*==================== 4) join ====================*/
// конкатенира елементите на вектора със сепаратор
string join(const vector<string>& parts, const string& sep) {
    if (parts.empty()) return "";
    string out;
    out.reserve(64); // малка оптимизация
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i) out += sep;
        out += parts[i];
    }
    return out;
}

/*==================== 5) Timer (std::chrono) ====================*/
class Timer {
public:
    void start() {
        if (!running_) {
            running_ = true;
            start_ = clock::now();
        }
    }
    void stop() {
        if (running_) {
            accum_ += clock::now() - start_;
            running_ = false;
        }
    }
    // връща изминалото време в секунди (double)
    double elapsed() const {
        using std::chrono::duration_cast;
        using std::chrono::duration;
        auto span = accum_;
        if (running_) {
            span += (clock::now() - start_);
        }
        return std::chrono::duration<double>(span).count();
    }
    void reset() {
        running_ = false;
        accum_ = clock::duration::zero();
    }

private:
    using clock = std::chrono::steady_clock;
    bool running_ = false;
    clock::time_point start_{};
    clock::duration accum_{};
};

/*——— Демонстрация: подаване по const& срещу по стойност ———*/
// „Тежък“ тип за демонстрация (голям вектор)
using Heavy = std::vector<int>;

// обработка по const& (избягва копие)
long long processConstRef(const Heavy& v) {
    long long sum = 0;
    for (int x : v) sum += x;
    return sum;
}

// обработка по стойност (прави копие на целия вектор)
long long processByValue(Heavy v) {
    // за да не е „мъртъв“ код, модифицираме локалното копие:
    if (!v.empty()) v[0] += 1;
    long long sum = 0;
    for (int x : v) sum += x;
    return sum;
}

/*==================== main: бърза демонстрация ====================*/
int main() {
    cout << "=== 1) Date & isLeap ===\n";
    Date dt{1, 3, 2024};
    cout << dt.y << " е " << (isLeap(dt) ? "високосна" : "невисокосна") << " година.\n\n";

    cout << "=== 2) Student normalize/print ===\n";
    Student s{"Ivan Petrov", 12345, 6.75};
    normalize(s); // ще стане 6.0
    print(s);
    s.grade = 1.50;
    normalize(s); // ще стане 2.0
    print(s);
    cout << "\n";

    cout << "=== 3) Клас Vector<double> (Rule of 5) ===\n";
    Vector v(5);
    for (size_t i = 0; i < v.size(); ++i) v[i] = static_cast<double>(i) * 1.5;
    const Vector& cv = v;
    cout << "Размер: " << cv.size() << ", елемент [3] = " << cv[3] << "\n";
    Vector v2 = v;           // копие
    Vector v3 = std::move(v); // преместване
    cout << "v2[3]=" << v2[3] << ", v3[3]=" << v3[3] << "\n\n";

    cout << "=== 4) join(vector<string>, sep) ===\n";
    vector<string> words = {"C++", "is", "fast", "and", "powerful"};
    cout << join(words, " ") << "\n\n";

    cout << "=== 5) Timer + const& vs by value ===\n";
    // създаваме голям вектор за ефект
    Heavy big(5'000'000, 1);
    big[123] = 7; // малко раздвижване

    Timer t;
    t.start();
    auto sum1 = processConstRef(big);
    t.stop();
    cout << "processConstRef (const&) sum=" << sum1
         << ", time=" << t.elapsed() << " s\n";

    t.reset();
    t.start();
    auto sum2 = processByValue(big); // копиране на 5M елемента
    t.stop();
    cout << "processByValue (by value) sum=" << sum2
         << ", time=" << t.elapsed() << " s\n";

    cout << "\nОчаквано, подаването по const& е по-бързо (без копие), "
            "докато подаването по стойност копира целия обект.\n";

    return 0;
}
