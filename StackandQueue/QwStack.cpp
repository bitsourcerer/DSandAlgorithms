#include "QwStack.hpp"
#include <iostream>

using namespace std;

template class Queue<int>;

int main()
{
    Queue<double> dq;

    dq.enqueue(3.14);

    for(double i = 0.0; i < 5.75; i += 0.236) dq.enqueue(i);
    cout << dq.dequeue();

    for (int n = 0, sz = dq.size(); n < sz; ++n) cout << dq.dequeue() << ' ';
}
