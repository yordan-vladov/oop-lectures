#include <iostream>
using namespace std;

int main() {
    int x = 10;
    int* ptr = &x; // пойнтър към x

    cout << "Първоначална стойност: " << x << endl;

    *ptr = 25; // промяна на стойността чрез пойнтъра

    cout << "Нова стойност след промяна чрез пойнтър: " << x << endl;

    return 0;
}
