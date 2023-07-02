#pragma once

#include <array>
#include <stdexcept>

namespace ds
{

template <typename T>
class Array
{
public:
	Array() : rarray(nullptr), length(0) { resize(); };
	Array(unsigned n) : rarray(new T[n]), length(n) { update(n); };
	~Array() { free(); }

	void insert(const T &val, unsigned pos);
	T remove(unsigned pos);

	unsigned size() const;
	bool empty() const;

	const T& operator[](unsigned i) const;
	T& operator[](unsigned i);

	unsigned length;

private:
	T *rarray;
	void update(unsigned);
	void resize();
	void free();
};

template <typename T>
inline unsigned Array<T>::size() const
{
	return length;
}

template <typename T>
inline bool Array<T>::empty() const
{
	return length == 0;
}

template <typename T>
const T& Array<T>::operator[](unsigned i) const
{
	if(i >= length) throw std::logic_error("Arrays index out of bounds!\n");
	return rarray[i];
}

template <typename T>
T& Array<T>::operator[](unsigned i)
{
	if (i >= length && i < 2*length) resize();
	else if (i >= 2*length) throw std::logic_error("Array index out of bounds!\n" + std::to_string(length));
	return rarray[i];
}

template <typename T>
void Array<T>::update(unsigned n)
{
	length = n;
}

template <typename T>
void Array<T>::resize()
{
	unsigned cap = length ? 2 * length : 1;
	T* ranew = new T[cap];
	unsigned l = length;

	for(int i = 0; i < l; ++i) ranew[i] = std::move(rarray[i]);
	free();
	rarray = ranew;
	update(cap);
}

template <typename T>
void Array<T>::free()
{
	if(rarray) delete[] rarray;
	rarray = nullptr;
	length = 0;
}

}