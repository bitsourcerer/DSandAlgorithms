#pragma once
#include "common.hpp"
#include <functional>
#include <cassert>

namespace algo
{

namespace sorts
{

template <class C, typename P = std::less<typename C::value_type>>
unsigned partition(C &con, unsigned lo, unsigned hi, P com = P())
{
	unsigned i = lo, j = hi+1;
	while(true)
	{
 		while(com(con[lo], con[--j])) //com stands for compare which is function object passed to this function
 			if (i == hi) break;

 		while(com(con[i], con[lo]))
 			if(j == lo) break;

 		if(i >= j) break; //if i and j crosses.
 		swap(con[i], con[j]);
 	}
 	swap(con[j], con[lo]);
 	return j;
}

template <class C, typename P = std::less<typename C::value_type>>
void qsort(C &con, unsigned lo, unsigned hi, P com = P())
{
	if(hi <= lo) return;
	unsigned j = partition(con, lo, hi);
	qsort(con, lo, j);
	qsort(con, j+1, hi);
}

template <class C, typename P = std::less<typename C::value_type>>
C& quick_sort(C& container, P comparison = P())
{
	qsort(container, 0, container.size() - 1, comparison);
	assert(is_sorted(container, 0, container.size()));
	return container;
}

} //sorts

} //algo