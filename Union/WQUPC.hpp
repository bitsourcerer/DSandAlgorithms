#ifndef WQUPC_HPP_INCLUDED
#define WQUPC_HPP_INCLUDED

#include "QuickUnion.hpp"

template <unsigned int S>
class WeightedQU : private QuickUnion<S>
{
    using QuickUnion<S>::id;
    using typename QuickUnion<S>::size_type;
public:
    WeightedQU() : QuickUnion<S>() { for(int &i : weights) i = 1; }
    size_type root(size_type c) override;
    bool connected(size_type p, size_type q) override;
    void make_union(size_type p, size_type q) override;

protected:
    std::array<int, S> weights;
};

template <unsigned int S>
typename WeightedQU<S>::size_type WeightedQU<S>::root(size_type c)
{
    while(c != this->id[c]) {
        /// Compress Path
        // Update the id of c index to the id of its parent.
        id[c] = id[id[c]];
        c = id[c];
    }
    return c;
}

template <unsigned int S>
bool WeightedQU<S>::connected(size_type p, size_type q) {
    return (root(p) == root(q));
}

template <unsigned int S>
void WeightedQU<S>::make_union(size_type p, size_type q)
{
    size_type rp = root(p), rq = root(q);

    if(weights[rp] > weights[rq]) { id[rq] = rp; weights[rp] += weights[rq]; }
    else { id[rp] = rq; weights[rq] += weights[rp]; }
}

#endif // WQUPC_HPP_INCLUDED
