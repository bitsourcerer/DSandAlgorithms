#ifndef QUICKUNION_HPP_INCLUDED
#define QUICKUNION_HPP_INCLUDED

#include <array>
#include <iostream>

template <unsigned int S>
class QuickUnion
{
public:
    using size_type = typename std::array<unsigned int, S>::size_type;
    QuickUnion();
    virtual ~QuickUnion() = default;
    virtual size_type root(size_type c);
    virtual bool connected(size_type p, size_type q);
    virtual void make_union(size_type p, size_type q);
    void print_id() const;
    void reset();

protected:
    std::array<unsigned int, S> id;
};

template <unsigned int S>
QuickUnion<S>::QuickUnion() : id()
{
    for(size_type i = 0; i < id.size(); ++i) id[i] = i;
}

template <unsigned int S>
typename QuickUnion<S>::size_type QuickUnion<S>::root(size_type c) {
    if(id[c] == c) return c;
    return root(id[c]);
}

template <unsigned int S>
bool QuickUnion<S>::connected(size_type p, size_type q)
{
    return (root(p) == root(q));
}

template <unsigned int S>
void QuickUnion<S>::make_union(size_type p, size_type q)
{
    //if(connected(p, q)) return; // no need to implement this.
    size_type rp = root(p), rq = root(q);
    id[rp] = rq;
    /// set the id of the root of p to root of q.
    /// since concept of root is absolute we donot have to check the id of rq too.
    /// because root means that the id of that component is equal to itself.
}

template <unsigned int S>
void QuickUnion<S>::print_id() const
{
    std::cout << "index:\t\t";
    for(uint32_t i = 0; i < id.size(); ++i) std::cout << i << ' ';

    std::cout << "\nid[index]:\t";
    for(uint32_t i = 0; i < id.size(); ++i) std::cout << id[i] << ' ';
    std::cout << std::endl;
}

template <unsigned int S>
void QuickUnion<S>::reset()
{
    for(size_type i = 0; i < S; ++i) id[i] = i;
}

#endif // QUICKUNION_HPP_INCLUDED
