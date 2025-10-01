#include <iostream>
using namespace std;

int main()
{
    unsigned int a = 0; // min value
    a -= 1; // overflow
    cout << a << endl; // max value 
}