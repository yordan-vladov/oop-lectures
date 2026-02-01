#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    int l,r;
    cin >> l >> r;

    for (int i = l; i <= r; i++)
    {
        if (i == 2 || i == 3)
        {
            cout << i << endl;
            break;
        }

        if (i % 2 == 0)
            continue;

        int limit = sqrt(i);

        bool found = true;
        for (int j = 3; j < limit; j += 2)
        {
            if (i % j == 0)
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            cout << i << endl;
            break;
        }
    }

    return 0;
}