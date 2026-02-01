#include <iostream>
using namespace std;

int main()
{
    short a = 32000;
    int b = 2000000000;
    long c = 4000000000;
    long long d = 4000000000;

    cout << "short size: " << sizeof(short);
    cout << "int size: " << sizeof(int);
    cout << "long size: " << sizeof(long);
    cout << "long long size: " << sizeof(long long);

    return 0;
}