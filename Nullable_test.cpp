#include "Nullable.hpp"
#include <iostream>

using namespace std;

int main()
{
	Nullable<int> ni1 = 666;
	cout << ni1 << endl;

	ni1 = null;
	cout << ni1 << endl;

	system("pause");
}