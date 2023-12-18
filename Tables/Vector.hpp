#pragma once

/**
*   @author Saqib Mehmood
*   @date 16/3/2023
*   Time  3:45 PM
*   C++ Primer 5th Edition Chapter 16
*   @note EXERCISE SECTION 16.1.2 PAGE 823
**/

#include <memory>
#include <utility>  // std::move, std::pair, std::forward
#include <string>
#include <vector>
#include <initializer_list>

//Exercise 16.16: Rewrite the StrVec class (§ 13.5, p. 526) as a template
//named Vec.

//Exercise 16.58: Write the emplace_back function for your StrVec class
//and for the Vec class that you wrote for the exercises in § 16.1.2 (p. 668).

template <typename> class Vector;
template <typename I> bool operator==(const Vector<I>&, const Vector<I>&);
template <typename I> bool operator<(const Vector<I>&, const Vector<I>&);

typedef Vector<std::string> StrVec;

template <typename T>
class Vector
{
    typedef T *iter_t;
    using Allocator = std::allocator<T>;
    using AllocTraits = std::allocator_traits<Allocator>;

public:
    typedef T value_type;
    typedef std::size_t size_type;
    Vector();
    Vector(const Vector&);
    Vector(Vector&&) noexcept;
    Vector(std::initializer_list<T>);
    Vector(const std::vector<T>&);
    Vector(size_type sz);
    ~Vector() { free(); }

    Vector& operator=(const Vector&);
    Vector& operator=(Vector&&) noexcept;
    Vector& operator=(const std::initializer_list<T>&);    // or assign() function
    Vector& operator=(const std::vector<T>&);

    void resize(size_type); //Not implemented yet
    void reserve(size_type);

    size_type capacity() const { return cap - first; }
    size_type size() const { return last - first; }
    bool empty() const { return first == last; /* OR last - first == (ptrdiff_t) 0 : OR return size();*/ }

    iter_t begin() const { return first; }
    iter_t end() const { return last; }

    void push_back(const T&);
    void push_back(T&&);
    template <class...Args> void emplace_back(Args&&...);
    void pop_back();
    void clear();

    friend bool operator==<T>(const Vector<T>&, const Vector<T>&);
    friend bool operator< <T>(const Vector<T>&, const Vector<T>&);

    explicit operator std::vector<T>() const { return std::vector<T>(begin(), end()); }
    value_type& operator[](size_type index) { return first[index]; /* OR *(start + index) */ }
    value_type& at(size_type index) { if(index >= capacity()) throw std::out_of_range("Index Unreachable"); return first[index]; }

private:
    T *first, *last, *cap;
    std::allocator<T> alloc;
    std::pair<T*, T*> alloc_copy(const T*, const T*);
    template <class C> void container_initialize(const C&, bool = false);
    void check_alloc();
    void reallocate();
    void free();
};
// template <typename, class C> std::pair<T*, T*> alloc_copy(typename C::const_iterator, typename C::const_iterator);
template <typename T>
Vector<T>::Vector() : first(nullptr), last(nullptr), cap(nullptr) {}

template <typename T>
Vector<T>::Vector(const Vector &obj) : first(obj.first), last(obj.first), cap(obj.cap) { ; }

template <typename T>
Vector<T>::Vector(Vector &&obj) noexcept : first(std::move(obj.first))
, last(std::move(obj.last)), cap(std::move(obj.cap)) { ; }

template <typename T>
Vector<T>::Vector(std::initializer_list<T> il)
{
    auto dat = alloc_copy(il.begin(), il.end());
    first = dat.first;
    last = cap = dat.second;
}

template <typename T>
Vector<T>::Vector(const std::vector<T> &vec)
{
    container_initialize(vec);
}

template <typename T>
Vector<T>::Vector(size_type sz)
{
    first = alloc.allocate(sz);
    last = std::uninitialized_fill_n(first, sz, value_type());
    cap = last;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector &rhs)
{
    auto newdata = alloc_copy(rhs.begin(), rhs.end());
    free();

    first = newdata.first;
    last = cap = newdata.second;

    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector &&rhs) noexcept
{
    if(this != &rhs)
    {
        free();
        first = rhs.first;
        last = rhs.last;
        cap = rhs.cap;

        rhs.first = rhs.last = rhs.cap = nullptr;
    }
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const std::vector<T> &vec)
{
    container_initialize(vec, true);
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const std::initializer_list<T> &il)
{
    auto newdata = alloc_copy(il.begin(), il.end());
    free();

    first = newdata.first;
    last = cap = newdata.second;

    return *this;
}

template <typename T> void Vector<T>::push_back(const T &val)
{
    check_alloc();
    AllocTraits::construct(alloc, last++, val);
}

template <typename T> void Vector<T>::push_back(T &&val)
{
    check_alloc();
    AllocTraits::construct(alloc, last++, std::move(val));
}

template <typename T>
template <class...Args> void Vector<T>::emplace_back(Args&&...args)
{
    check_alloc();
    AllocTraits::construct(alloc, last++, std::forward<Args>(args)...);
}

template <typename T> void Vector<T>::pop_back()
{
    if(size()) AllocTraits::destroy(alloc, --last);
}

template <typename T> void Vector<T>::clear()
{
    while(last != first) AllocTraits::destroy(alloc, --last);
}

template <typename T> void Vector<T>::resize(size_type n)
{
    if(n > size())
    {
        while(size() != n) emplace_back();
    }
    else
    while(size() != n) pop_back();
}

template <typename T>
std::pair<T*, T*> Vector<T>::alloc_copy(const T *b, const T *e)
{
    auto alloc_start = alloc.allocate(e - b);    // e - b is ptrdiff_t
    auto alloc_end = alloc_start;
    alloc_end = std::uninitialized_copy(b, e, alloc_end);

    return {alloc_start, alloc_end};
}

template <typename T>
template <class C> void Vector<T>::container_initialize(const C &container, bool free)
{
    typename C::const_iterator b = container.begin(), e = container.end();
    auto fptr = alloc.allocate(container.size());    // or e - b
    auto lptr = fptr;
    while(b != e)
        AllocTraits::construct(alloc, lptr++, *b++);

    if(free) this->free();
    first = fptr;
    last = cap = lptr;
}

template <typename T>
void Vector<T>::check_alloc()
{
    if(size() == capacity()) reallocate();  // or if last == cap;
}

template <typename T> void Vector<T>::free()
{
    if(first)   //if not null
    {
        for(auto p = last; p != first;)
            AllocTraits::destroy(alloc, --p);
    }
    alloc.deallocate(first, capacity());
}

template <typename T>
void Vector<T>::reallocate()
{
    size_type newcap = capacity() ? capacity() * 2 : 1;

    auto newstart = alloc.allocate(newcap);
    auto newend = newstart;

    //for (size_type i = 0; i != size(); ++i)   // need copy of first, first_copy
    //while(size())   // since size is last - first; bad practice problem for deallocation
    for(auto first_copy = first; first_copy != last; /* ++first_copy */)
        AllocTraits::construct(alloc, newend++, *first_copy++);    // or std::move(*first++); or use make_move_iterator
    free();

    first = newstart;
    last = newend;
    cap = newstart + newcap;
}

template <typename I>
bool operator==(const Vector<I> &lhs, const Vector<I> &rhs)
{
    return lhs.first == rhs.first && lhs.last == rhs.last;
}

template <typename I>
bool operator!=(const Vector<I> &lhs, const Vector<I> &rhs)
{
    return !(lhs == rhs);
}

template <typename I>
bool operator<(const Vector<I> &lhs, const Vector<I> &rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), lhs.end());
}

template <typename I>
bool operator>(const Vector<I> &lhs, const Vector<I> &rhs)
{
    return rhs < lhs;
}