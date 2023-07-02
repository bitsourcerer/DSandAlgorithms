#include "vector.hpp"
#include "resizing_array.hpp"
#include <cmath>
#include <functional>

namespace ds
{

using std::swap;

template <typename T, typename P>
class BinaryHeap
{
public:
	BinaryHeap() : heap_array(1) { }
	BinaryHeap(unsigned n) : heap_array(n+1) { }
	BinaryHeap(P comp) : compare(comp) { }

	void insert(const T &val);
	T remove();
	T top() const;

	unsigned height() const;
	unsigned size() const;

private:
	P compare;
	// ResizingArray<T> heap_array;
	Array<T> heap_array;
	unsigned N = 0;

	void sink(unsigned key);
	void swim(unsigned key);
};

template <typename T, typename P>
void BinaryHeap<T, P>::insert(const T &val)
{
	//discard N = 0; start from N = 1;
	heap_array[++N] = val;
	swim(N);
}

template <typename T, typename P>
T BinaryHeap<T, P>::remove()
{
	if(N == 0) throw std::invalid_argument("remove() on empty heap!");
	T ret = heap_array[1];
	swap(heap_array[1], heap_array[N--]);
	sink(1);

	return ret;
}

template <typename T, typename P>
inline T BinaryHeap<T, P>::top() const
{
	if(N == 0) throw std::logic_error("top() on empty heap!");
	return heap_array[1];
}


#include <iostream>
template <typename T, typename P>
inline unsigned BinaryHeap<T, P>::height() const
{
	for (int i = 1; i <= N; ++i)
		std::cout << heap_array[i] << ' ';
	std::cout << std::endl;
	return std::floor(std::log(N)); //log base 2 of size of the heap array.
	// and floor it.
}

template <typename T, typename P>
inline unsigned BinaryHeap<T, P>::size() const
{
	return N; // exclude first unused location (index 0);
}

template <typename T, typename P>
void BinaryHeap<T, P>::swim(unsigned key)
{
	while(key > 1 && compare(heap_array[key] , heap_array[key/2])) // key > 1 makes sure swim cant
	// run on empty heap and also terminates the loop.
	{
		swap(heap_array[key/2], heap_array[key]); // swap with its parent
		key /=2;
	}
}

template <typename T, typename P>
void BinaryHeap<T, P>::sink(unsigned key)
{
	while(2*key <= N) //check if node has kids.
	{
		unsigned node = 2 * key; //points to the kids(nodes or childs);

		if(node < N && compare(heap_array[node+1], heap_array[node])) ++node; //MinPQ if right child is smaller than
		// the left one then set priority child as the right one and proceed.
		//if(compare(heap_array[node], heap_array[node + 1])) ++node;
		//compare both the kids at 2*key and 2*key + 1;
		if(!compare(heap_array[node], heap_array[key])) break;
		// if(compare(heap_array[key], heap_array[node])) break; //SIMILAR if key is smaller than child already.
		//for MinPQ if node(child) isnt smaller than key(its parent) then donot swap. opposite for MaxPQ

		swap(heap_array[key], heap_array[node]);
		key = node;
	}
}

} //ds