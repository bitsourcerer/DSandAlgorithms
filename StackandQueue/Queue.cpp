#include "Queue.hpp"
#include <iostream>
#include <vector>

using namespace std;

template class Queue<int>;

int main()
{
    cout << boolalpha;
    Queue<int> iq;

    for(int i = 0; i < 100; ++i) iq.push(std::move(i));

    for(const int i : iq) cout << i << ' ';
    cout << '\n' << iq.size() << '\n';
    cout << iq.capacity() << '\n';

    cout << iq.front() << '\n';
    cout << iq.back() << '\n';

    iq.fill(10, 8);
    for(const int i : iq) cout << i << ' ';
    cout << '\n' << iq.size() << '\n';
    cout << iq.capacity() << '\n';

    for(int i = 0, sz = iq.size(); i < sz; ++i) cout << iq.pop() << " popped\n";
    cout << iq.empty() << '\n';

    for(const int i : iq) cout << i << ' ';

    cout << "\n----------------------------------------------------\nQueue<double>\n" << boolalpha;
    Queue<double> dq; cout << dq.empty() << '\n';
    dq.fill(10);
    for(double d : dq) cout << d << ' ';
    cout << endl << dq.capacity() << '\n';
    cout << dq.size() << '\n';
    cout << dq.empty() << '\n';

    //dq.push(3.14);

    dq.fill(100, 3.14);
    cout << endl << dq.capacity() << '\n';
    cout << dq.size() << '\n';
    cout << dq.empty() << '\n';

    dq.clear();
    cout << endl << dq.capacity() << '\n';
    cout << dq.size() << '\n';
    cout << dq.empty() << '\n';

    for(double d : dq) cout << d << ' ';
}
