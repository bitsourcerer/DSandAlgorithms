#include "Stack.hpp"
#include <iostream>
#include "../../Primer/ch16/ex_16_16_Vector.h"
#include <vector>

using namespace std;

int main()
{
    Stack<int> intstack;
    intstack.push(10);
    intstack.push(8);
    intstack.pop();

    cout << intstack.top() << '\n';

    for(int i = 0; i < 200; ++i)
    {
        intstack.push(i);
        std::cout << intstack.top() << ' ';
    }

    Stack<int> istack2(intstack);
    //Stack<int> istack3(std::move(intstack));

    for(int i = 0; i < 200; ++i) intstack.pop();
    cout << "\n#__________________________________________________________________#" << endl;

    for(int i : intstack)
    {
        cout << i << ' ';
    }
    cout << "\n#__________________________________________________________________#" << endl;
    //for(int i : istack3) cout << i << ' ';
    system("pause");
}

