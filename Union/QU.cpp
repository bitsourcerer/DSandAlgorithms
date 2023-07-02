#include "QuickUnion.hpp"

using namespace std;

int main()
{
    QuickUnion<10> qu;

    qu.make_union(3,2); qu.make_union(6,2);
    qu.make_union(2,8);
    qu.print_id(); cout << endl;

    qu.make_union(0, 4); qu.make_union(1, 4);
    qu.make_union(2, 4); // now 8s root should be 4;
    qu.print_id(); cout << endl;
    cout << qu.root(6);
}
