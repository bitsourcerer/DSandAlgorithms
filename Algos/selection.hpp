#include "quick_sort.hpp"
#include <algorithm>
#include <random>

namespace algo
{

using sorts::partition;

/** Find kth smallest integer in an array.
* after partition all the elements to the left are smaller than j and to the right are greater.
* so to the left of j there are total of j elements (considering index starts from 0) that are
* smaller than the element at index j and to the right there are (size - j) elements that are greater,
* if k is smaller than the index j since j is jth smallest element so it implies that the rank we are
* looking for is even smaller than j, therefore we would have to move to the left side of array, and search
* for the rank there, because of the nature of quicksort we would have to repeat the procedure on the left
* side, and the then retrieved value of j(index) matches the k then we return the element at j i.e array[j].
* if its greater than j which again implies that the rank is greater than j so we move to the right half of the
* then half of the array, else we move left.
**/
template <typename C>
typename C::value_type selection(C &con, typename C::size_type k)
{
	int lo = 0, hi = con.size() - 1;
	while(hi > lo)
	{
		unsigned rs = partition(con, lo, hi);

		if(rs > k) hi = rs - 1;
		else if(rs < k) lo = rs + 1;
		else return con[k];
	}
	return con[k];
}

template <typename C>
typename C::value_type select(const C &con, unsigned k)
{
	C cp(con);
	std::shuffle(cp.begin(), cp.end(), std::default_random_engine(std::random_device()()));
	return selection(cp, k);
}

} //algo