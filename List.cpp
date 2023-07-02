#include "LinkedList.h"

using namespace std;

template class LinkedList<int>;

int main()
{
    LinkedList<int> lst;
    lst.insert(50);
    auto ret2 = lst.insert(80);
    auto ret = lst.insert(66);

    lst.assign(ret, 88);
    lst.assign(ret2, 44);

    lst.traverse();
    auto found = lst.find(88);
    cout << "data from found node: " << found.data << '\n';

    lst.traverse();
}
