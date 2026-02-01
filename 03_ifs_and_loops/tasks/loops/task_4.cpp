#include <iostream>
using namespace std;

int main()
{
    int n,k;

    cout << "Enter n: ";
    cin >> n;

    cout << "Enter k: ";
    cin >> k;

    for (int i = k; i <= n; i++)
        if (i % k == 0)
            cout << i << " ";
    
    cout << endl;
    
    return 0;
}