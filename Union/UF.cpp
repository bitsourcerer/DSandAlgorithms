#include "UnionFind.hpp"

using namespace std;

int main()
{
    UnionFind<10> uf;
    cout << boolalpha;

    cout << uf.connected(1, 3) << endl;
    uf.make_union(1, 3);
    cout << '\n' << uf.connected(1, 3) << '\n' << endl;
    uf.reset();

    uf.make_union(4, 3); uf.make_union(3, 8);
    uf.make_union(6, 5); uf.make_union(9, 4);
    uf.make_union(2, 1); uf.make_union(8, 9);
    uf.make_union(5, 0); uf.make_union(7, 2);
    uf.make_union(6, 1);
    uf.print_id();
}

///1 1 1 8 8 1 1 1 8 8
///0 1 1 3 3 0 1 1 3 3
