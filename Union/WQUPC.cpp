#include "WQUPC.hpp"
#include <iostream>

using namespace std;

int main()
{
    WeightedQU<10> wqupc;
    cout << boolalpha << wqupc.connected(1, 3) << endl;
    wqupc.make_union(3, 1);
    cout << boolalpha << wqupc.connected(1, 3) << endl;
}
