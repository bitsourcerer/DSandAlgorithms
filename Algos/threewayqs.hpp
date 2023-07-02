#pragma once
#include "common.hpp"

namespace algo {

namespace sorts
{

// Using 3 way partition by Edsger Dijkstra
template <class C>
void twqsort(C &con, int lo, int hi)
{
	if(hi <= lo) return;

	int lt = lo, gt = hi;
	unsigned i = lo;

	typename C::value_type v = con[lo]; //should'nt be a reference!
	while(i <= gt)
	{
		auto c = con[i];
 		if(c < v) swap(con[i++], con[lt++]);
 		else if(c > v) swap(con[i], con[gt--]);
 		else ++i;
 	}
 	twqsort(con, lo, lt-1);
 	twqsort(con, gt+1, hi);
}


template <class C>
C quick_sort(const C& container)
{
	C cop(container);
	twqsort(cop, 0, cop.size() - 1);
	assert(is_sorted(cop, 0, cop.size()));
	return cop;
}

//-------------------------------------ARRAY-------------------------------------//
template <typename T, unsigned S>
void twqsort(T (&arr)[S], int lo, int hi)
{
	if(hi <= lo) return;
	T v = arr[lo];

	int lt = lo, gt = hi;
	unsigned i = lo;
	while(i <= gt)
	{
		T c = arr[i];
 		if(c < v) swap(arr[i++], arr[lt++]);
 		else if(c > v) swap(arr[i], arr[gt--]);
 		else ++i;
 	}
 	twqsort(arr, lo, lt-1);
 	twqsort(arr, gt+1, hi);
}


template <typename T, unsigned S>
auto quick_sort(T (&arr)[S]) -> decltype(arr)
{
	unsigned sz = static_cast<unsigned>(extent<std::remove_reference_t<decltype(arr)>>::value);
	//unsigned sz = S;
	assert(sz == 20);
	twqsort(arr, 0, sz - 1);
	//assert(is_sorted(arr, 0, sz-1));
	return arr;
}


} // sorts
// NAMESPACES
} // algo