#ifndef QUEUE_HPP_INCLUDED
#define QUEUE_HPP_INCLUDED

#include <type_traits>
#include <memory>
#include <cassert>
//#include <iostream>

template <typename T> using UnityPair = std::pair<T, T>;

template <typename> class Queue;

template <typename T>
class Queue
{
    static_assert(std::is_same<typename std::remove_cv<T>::type, T>::value, // if they arent same assertion fails!
        "Logic Error only non-const and non-volatile Type can be used as Queue template argument!");

    friend class Queue<const T>;
    using value_type = typename std::remove_reference<T>::type;
    using size_type = std::size_t;
    using reference = typename std::add_lvalue_reference<T>::type;
    using iterator = typename std::add_pointer<value_type>::type;
    using alternate_type = typename std::conditional< std::is_const<T>::value, value_type, const value_type >::type;
    using allocator_type = typename std::remove_cv<value_type>::type;

public:
    Queue();
    Queue(const Queue&);
    template <typename X> Queue(const Queue<X>&);
    template <class Iter> Queue(Iter, Iter);
    ~Queue();

    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type capacity() const noexcept;

    template <typename ... Args>
    void emplace(Args&&...);
    void push(const value_type&);
    void push(value_type&&);
    value_type pop();

    void fill(size_type, value_type = value_type());
    void clear();

    inline reference front();
    inline reference back();

    inline iterator begin() const;
    inline iterator end() const;

    //operator Queue<alternate_type>() const { return *reinterpret_cast<const Queue<alternate_type>*>(this); }
    // or simply *this; if there is ctor that takes Queue<const value_type>;

private:
    value_type *start, *finish, *pcap;
    mutable std::ptrdiff_t cap_size, bias;
    std::allocator<allocator_type> mem;

    template <class Iter>
    UnityPair<iterator> copyMemory(Iter, Iter);
    void adjustMemory();
    void checkMemory();
    void freeMemory();
};

template <typename T>
Queue<T>::Queue() : start(nullptr), finish(nullptr), pcap(nullptr), cap_size(0), bias()
{
    static_assert(std::is_same<typename std::remove_cv<T>::type, T>::value,
        "Logic Error only non-const and non-volatile Type can be used as Queue template argument!");
}

template <typename T>
Queue<T>::Queue(const Queue &obj) : Queue()
{
    UnityPair<iterator> allocation = copyMemory(obj.begin(), obj.end());

    start = pcap = allocation.first;
    finish = allocation.second;
}

template <typename T>
template <typename X>
Queue<T>::Queue(const Queue<X> &obj) : Queue()
{
    if(!std::is_same<T, typename Queue<X>::alternate_type>::value) return;
    UnityPair<iterator> allocation = copyMemory(obj.begin(), obj.end());

    start = pcap = allocation.first;
    finish = allocation.second;
}

/*
template <typename T>
Queue<T>::Queue(const Queue<typename std::add_const<T>::type> &obj) : Queue()
{
    UnityPair<iterator> allocation = copyMemory(obj.begin(), obj.end());

    start = pcap = allocation.first;
    finish = allocation.second;
}
*/

template <typename T>
template <class Iter>
Queue<T>::Queue(Iter _begin, Iter _end) : Queue() // : cap_size(_end - _begin)
{
    UnityPair<iterator> allocation = copyMemory(_begin, _end);

    start = pcap = allocation.first;
    finish = allocation.second;
}

template <typename T>
Queue<T>::~Queue() { freeMemory(); }

/** SIZE RELATED **/
template <typename T>
bool Queue<T>::empty() const noexcept
{
    return !size(); // OR return (size() ? false : true);
}

template <typename T>
typename Queue<T>::size_type Queue<T>::size() const noexcept
{
    //static size_type sz;
    //if(!start || !finish) return 0;
    return finish - start;
}

template <typename T>
typename Queue<T>::size_type Queue<T>::capacity() const noexcept
{
    return cap_size;
}

/** VALUE RELATED **/
template <typename T>
template <typename...Args>
void Queue<T>::emplace(Args&&...args)
{
    checkMemory();
    mem.construct(finish++, std::forward<Args>(args)...);
}

template <typename T>
void Queue<T>::push(const value_type &val)
{
    checkMemory();
    mem.construct(finish++, val);
}

template <typename T>
void Queue<T>::push(value_type&& val)
{
    checkMemory();
    mem.construct(finish++, std::move(val));
}

template <typename T>
typename Queue<T>::value_type Queue<T>::pop()
{
    if(empty()) return value_type();

    value_type ret = front();
    mem.destroy(start++);
    bias = start - pcap;  // OR ++bias;

    return ret;
}

/** QUERY VALUES **/
template <typename T>
typename Queue<T>::reference Queue<T>::front()
{
    return start[0]; // or *start;
}

template <typename T>
typename Queue<T>::reference Queue<T>::back()
{
    return finish[-1];
}

template <typename T>
void Queue<T>::fill(size_type n, value_type v)
{
    //checkMemory(); SERIOUS ERROR! Just kidding
    if(n <= capacity())
    {
        //for(auto p = start; p != start + n; *p = val, ++p);
        //for(size_type i = 0; i < n; ++i) start[i] = v;
        for(auto it = begin(); it != begin() + n; ++it) *it = v;
        //std::cout << "Difference : " << (end() - (begin() + n)) << "\t" << start[n] << std::endl;
        while(end() > begin() + n) mem.destroy(--finish);
    }
    else
    {
        iterator new_beg = mem.allocate(n);
        iterator new_end = std::uninitialized_fill_n(new_beg, n, v);

        freeMemory();

        start = pcap = new_beg;
        finish = new_end;
        bias = 0;
        cap_size = n; // cap_size = size();
    }
}

template <typename T>
void Queue<T>::clear()
{
    if(empty()) return;
    for(size_type i = 0, s = size(); i != s; ++i) mem.destroy(--finish);

    assert(finish == start && "Begin() must equal to End()");
    std::ptrdiff_t dealloc_n = cap_size - bias;

    /** THROWS ERROR **/
    //mem.deallocate(pcap, cap_size);
    //start = pcap;
    //cap_size = finish - pcap;
}

/** ITERATORS **/
template <typename T>
typename Queue<T>::iterator Queue<T>::begin() const
// const for begin and doesnt matter as long as managing type itself is pointer.
{
    return start;
    //return std::iterator<std::bidirectional_iterator_tag, value_type>(start);
}

template <typename T>
typename Queue<T>::iterator Queue<T>::end() const
{
    return finish;
}

/** PRIVATE UTILITY **/
template <typename T>
template <class Iter>
UnityPair<typename Queue<T>::iterator> Queue<T>::copyMemory(Iter b, Iter e)
{
    cap_size = e - b;
    auto new_start = mem.allocate(cap_size);
    auto new_finish = std::uninitialized_copy(start, finish, new_start);
    return {new_start, new_finish};
}

template <typename T>
void Queue<T>::checkMemory()
{
    if(size() == capacity()) adjustMemory();
}

template <typename T>
void Queue<T>::adjustMemory()
{
    size_type new_cap = (capacity() ? size() * 2 : 1);

    /** BIAS ADJUSTMENT **/
    if(size() && bias) //if(size() && bias >= (size() * 1.35))// just a magic number
    {
        std::copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), pcap);
        start = pcap;

        //finish -= bias;
        for(auto p = finish; p != finish - bias;) mem.destroy(--finish);
        return;
    }

    value_type* new_start = mem.allocate(new_cap), *new_finish = new_start;
    for(auto p = start; p != finish;)
        mem.construct(new_finish++, std::move(*p++));

    freeMemory();

    start = pcap = new_start;
    finish = new_finish;

    bias = 0;
    cap_size = new_cap;
}

template <typename T>
void Queue<T>::freeMemory()
{
    //if(!start || !finish) return;
    /*std::cout << "Freed Memory: " << size() * sizeof(value_type) << " Bytes\t"
    << "Total Slots: " << capacity() << std::endl;*/
    for(iterator p = finish; p != start;) mem.destroy(--p);
    mem.deallocate(pcap, cap_size);
}

#endif // QUEUE_HPP_INCLUDED
