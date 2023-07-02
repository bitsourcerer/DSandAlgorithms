#include "vector.hpp"
#include "resizing_array.hpp"
#include <iostream>

template class ds::Array<int>;

using namespace std;
using namespace ds;

int main()
{
	ResizingArray<int> ra(10);
	ra.insert(12);
	cout << ra[0];

	for(int i = 0; i < 50; ++i) ra.insert(i);
	for(int i : ra) cout << i << ' ';
	cout << endl;

	auto N = ra.length;
	
	cout << "\nAfter removing half elements\n";
	while(ra.length >= N/2) ra.remove();
	for(int i : ra) cout << i << ' ';
	cout << endl;

	while(!ra.empty) ra.remove();
	cout << "\nAfter emptying array\n";
	for(int i : ra) cout << i << ' ';
	cout << endl;


	cout << "\nArray\n";
	Array<int> rarray(10);
	cout << '\n' << rarray.length << endl;
	for(int i = 0; i < 20; ++i) rarray[i] = i;
	rarray[19] = 20;
	cout << '\n' << rarray.length << endl;

	for(int i = 0; i < 20; ++i) cout << rarray[i] << ' ';

	system("pause");
}