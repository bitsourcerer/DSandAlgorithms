#pragma once

#include "binaryheap.hpp"

namespace ds {

template <typename Type, typename Comp>
class PriorityQueue
{
	static_assert(std::is_same_v<Type, std::remove_cv_t<Type>>, "Logic Error Type cannot be Const or volatile.");
	using ElemType = std::remove_reference_t<Type>;
public:
	PriorityQueue() = default;
	PriorityQueue(Comp comparator) : heap(comparator) { }

	void insert(const ElemType&);
	ElemType remove();
	ElemType peek() const;	// formerly called next()
	unsigned size() const;

private:
	BinaryHeap<ElemType, Comp> heap;
};


template <typename T, typename P>
inline void PriorityQueue<T, P>::insert(const ElemType &val)
{
	heap.insert(val);
}

template <typename T, typename P>
inline typename PriorityQueue<T, P>::ElemType PriorityQueue<T, P>::remove()
{
	return heap.remove();
}

template <typename T, typename P>
inline typename PriorityQueue<T, P>::ElemType PriorityQueue<T, P>::peek() const
{
	return heap.top();
}

template <typename T, typename P>
inline unsigned PriorityQueue<T, P>::size() const
{
	return heap.size();
}

template <typename T> using MaxPQ = PriorityQueue<T, std::greater<T>>;
template <typename T> using MinPQ = PriorityQueue<T, std::less<T>>;

} //ds

