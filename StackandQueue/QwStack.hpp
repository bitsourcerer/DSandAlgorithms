#ifndef QWSTACK_HPP_INCLUDED
#define QWSTACK_HPP_INCLUDED

#include "Stack.hpp"

template <typename T>
class Queue
{
    using Stack_t = Stack<T>;

public:
    void enqueue(const T &val) {
        inbox.push(val);
    }

    T dequeue() {
        if(outbox.empty())
            while(!inbox.empty()) outbox.push(inbox.pop());
        return outbox.pop();
    }

    std::size_t size() const {
        return inbox.size() + outbox.size();
    }

    bool empty() const noexcept {
        return inbox.empty() && outbox.empty();
    }

private:
    Stack<T> inbox;
    Stack<T> outbox;
};

#endif // QWSTACK_HPP_INCLUDED
