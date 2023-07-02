/* Stack Implementation */

#include <memory>
#include <type_traits>
#include <utility>
#include <stack>

template <typename T> using Pair = std::pair<T, T>;

template <typename T>
class Stack
{
public:
    using value_type = typename std::remove_reference<T>::type;
	using reference = value_type&;
	using pointer = value_type*;
	using iterator = pointer;
	using const_iterator = const value_type*;
	typedef std::size_t size_type;

	Stack();
	Stack(const Stack&);
	Stack(Stack&&) noexcept;
	Stack& operator=(const Stack&);
	Stack& operator=(Stack&&);
	~Stack();

	void push(const T&);
	void push(T &&val);
	value_type pop();
	inline reference top();

	bool empty() const { return start == finish; /* size() == 0; */ }
	size_type size() const { return finish - start; }
	size_type capacity() const { return cap - start; }

    inline iterator begin() const; // since start and finish are pointers changing underlying
    inline iterator end() const; // value wont affect constness of object

private:
	std::allocator<T> mem;
	pointer start, finish, cap;
	value_type *&ptop = finish, *&pbottom = start;

	void reallocate();
	inline void check_allocate();

	template <class C>
	Pair<pointer> copy_memory(typename C::const_iterator, typename C::const_iterator);
	template <class C>
    Pair<pointer> copy_memory_from(C&&);
	void free_memory();
};

template <typename T>
Stack<T>::Stack() : start(nullptr), finish(nullptr), cap(nullptr) { }

template <typename T>
Stack<T>::Stack(const Stack &obj) : Stack()
{
    auto allocated = copy_memory<Stack>(obj.begin(), obj.end());
    start = allocated.first;
    finish = cap = allocated.second;
}

template <typename T>
Stack<T>::Stack(Stack &&obj) noexcept : start(std::move(obj.start)), finish(std::move(obj.finish)), cap(std::move(obj.cap))
{
    obj.start = obj.finish = obj.cap = nullptr; // not necessary
}

template <typename T>
Stack<T>::~Stack()
{
    free_memory();
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack &rhs)
{
    auto allocated = copy_memory(rhs.begin(), rhs.end());
    start = allocated.first;
    finish = cap = allocated.second;
}

template <typename T>
void Stack<T>::push(const T &val)
{
    check_allocate();
    mem.construct(finish++, val);
}

template <typename T>
void Stack<T>::push(T &&val)
{
    check_allocate();
    mem.construct(finish++, std::move(val));
}

template <typename T>
typename Stack<T>::value_type Stack<T>::pop()
{
    //if(size()) mem.destroy(--finish);
    value_type ret = top();
    if(empty()) return value_type();
    mem.destroy(--finish);
    return ret;
}

template <typename T>
typename Stack<T>::reference Stack<T>::top()
{
    //if(empty()) return nullptr;
    return finish[-1]; //*(finish - 1); // the top most element in stack and last in array memory!
}

template <typename T>
typename Stack<T>::iterator Stack<T>::begin() const
{
    return pbottom;
}

template <typename T>
typename Stack<T>::iterator Stack<T>::end() const
{
    return ptop;
}

template <typename T>
template <class C>
Pair<typename Stack<T>::pointer> Stack<T>::copy_memory(typename C::const_iterator b, typename C::const_iterator e)
{
    std::ptrdiff_t distance = e - b;
    auto alloc_begin = mem.allocate(distance);
    auto alloc_end = std::uninitialized_copy(b, e, alloc_begin);

    return {alloc_begin, alloc_end};
}

template <typename T>
void Stack<T>::reallocate()
{
    auto new_cap = size() ? size() * 2 : 1;
    pointer new_begin = mem.allocate(new_cap);
    pointer new_end = std::uninitialized_copy(std::make_move_iterator(start), std::make_move_iterator(finish) , new_begin);
    //for(auto p = start; p != finish; ++p) mem.construct(new_end++, std::move(*p));
    free_memory();

    start = new_begin;
    finish = new_end;
    cap = new_begin + new_cap;
}

template <typename T>
void Stack<T>::free_memory()
{
    if(!start) return;
    for(auto p = finish; p != start;) mem.destroy(--p);
    //while(size() && start != finish) mem.destroy(--finish);

    mem.deallocate(start, capacity());
    //start = finish = cap = nullptr;
}

/*
template <typename T>
template <class C>
Pair<typename Stack<T>::pointer> Stack<T>::copy_memory_from(C &&container)
{
    auto b = container.begin(), e = container.end();
    std::ptrdiff_t distance = e - b;
    auto alloc_begin = mem.allocate(distance);
    decltype(alloc_begin) alloc_end;

    if(std::is_lvalue_reference<T>{})
        alloc_end = std::uninitialized_copy(std::make_move_iterator(b), std::make_move_iterator(e), alloc_begin);
    else
        alloc_end = std::uninitialized_copy(b, e, alloc_begin);

    return {alloc_begin, alloc_end};
}
*/

template <typename T>
void Stack<T>::check_allocate()
{
    if(capacity() == size()) reallocate();
}
