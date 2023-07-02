#include <vector>
#include <cstddef>
#include <string>
#include <stdexcept>

template <typename T>
class ResizingArray : private std::vector<T>
{
	using base = std::vector<T>;
	using size_t = typename base::size_type;
public:
	ResizingArray() : base() { update_length(); }
	ResizingArray(size_t n) : base(n) { update_length(); }
	void insert(const T &val, size_t pos = END);
	T remove(size_t pos = END);

	T& operator[](size_t i);
	const T& operator[](size_t i) const;

	using base::begin;
	using base::end;

	size_t length;
	bool empty;

private:
	enum Pos : unsigned
	{
		START = 0xfffffffe,
		END
	};
	void update_length();
	void resize(unsigned);
};

template <typename T>
void ResizingArray<T>::insert(const T &val, size_t pos)
{
	if(pos == END) this->push_back(val);
	else{
		auto it = this->begin() + pos;
		base::insert(it, val);
	}
	update_length();
}

template <typename T>
T ResizingArray<T>::remove(size_t pos)
{
	T ret = this->back();;
	if(pos == END) this->pop_back();
	else this->erase(this->begin() + pos);

	update_length();
	return ret;
}

#include <iostream>
template <typename T>
void ResizingArray<T>::update_length()
{
	// length = this->size();
	length = base::size();
	//empty = length ? false : true;
	empty = base::empty();
}

template <typename T>
const T& ResizingArray<T>::operator[](size_t i) const
{
	return this->at(i);
}

template <typename T>
T& ResizingArray<T>::operator[](size_t i)
{
	resize(i); //if(length == 0 || (i >= length && i < 2*length))
	if(i >= length*2)
		throw std::logic_error(
			"Array index out of bounds!\n\tfor length " +
			std::to_string(length) + " and index " + std::to_string(i));
	return this->at(i); //return operator[](i); base::operator[](i);
}

template <typename T>
void ResizingArray<T>::resize(unsigned n)
{
	if(n < length) return;
	unsigned sz = empty ? n+1 : length * 2;
	base::resize(sz);
	update_length();
}

/**
#include <iostream>
template <typename T>
void ResizingArray<T>::resize(void)
{
	unsigned sz = length ? length * 2 : 1;
	base::resize(sz);
	update_length();
}


template <typename T>
void ResizingArray<T>::resize(unsigned n)
{
	// std::cout << length << '\t' << n << std::endl;
	if(n < length) return;
	unsigned sz = length ? length * 2 : 1;
	base::resize(sz);
	update_length();
}
**/