#ifndef UNIONFIND_HPP_INCLUDED
#define UNIONFIND_HPP_INCLUDED

#include <array>
#include <iostream>
#include <iterator>
#include <algorithm>

//auto get_range(size_t b, size_t e) -> std::array<unsigned int, e - b>
//{
//    std::array<unsigned int, e - b> res;
//    for(decltype(res)::size_type i = 0; i < e; ++i) res[i] = i;
//    return res;
//}
//
//auto range = std::bind(get_range, 0, std::placeholders::_1);

template <unsigned int sz>
class UnionFind
{
    using size_type = typename std::array<unsigned int, sz>::size_type;
public:
    UnionFind();
    bool connected(size_type, size_type) const;
    void make_union(size_type, size_type);
    void print_id() const;
    void reset();

private:
    std::array<unsigned int, sz> id;
};

template <unsigned int sz>
UnionFind<sz>::UnionFind(): id()
{
    for(size_type i = 0; i < sz; ++i) id[i] = i;
}

template <unsigned int sz>
bool UnionFind<sz>::connected(size_type p, size_type q) const
{
    return id[p] == id[q];
}

template <unsigned int sz>
void UnionFind<sz>::make_union(size_type p, size_type q)
{
    if(connected(p, q)) return;
    size_type idp = id[p], idq = id[q];
    /**
    If we donot store id of p in a seperate identifier then there
    is an error we are making, the loop will traverse through whole array
    and checks condition if(id[i] == id[p]) id[i] = id[q];
    lets say we are on i = p itself, and we check condition and it returns true,
    we set the id of p itself to id[q] but as we go further we still keep checking
    the condition which everytimes checks id of p and id of q (updated and not the original)
    so given we have already set the value of id[p] to id[q] so after that we are overriding
    all other ids that are equal to id[p] which is itself equal to id[q] to id[q].
    that way if there are numbers with id equal to id[p] after we surpassed the p, those number
    wouldnt update their ids and so they will remain same.
    0 3 2 3 4 5 6 7 8 9     here if we try to connect 1 to 8 in erroneous form it will look like
    we will update 1s id to 8s id, because it comes first so, 0 8 2 3 4 5 6 7 8
    as it is previously connected to 3 so iterating further and when we get to 3 we wont
    be able to change its id to 8s too, because it will check wether id[3] is id[p](id[1] which is 8)
    but the assertion will fail as 3 != 8 so id[3] will remain same.
    **/

    for(size_type i = 0; i < id.size(); ++i)
        if(id[i] == idp) id[i] = idq;
}

template <unsigned int sz>
void UnionFind<sz>::print_id() const
{
    static std::ostream_iterator<unsigned int> id_out(std::cout, " ");

    std::cout << "index:\t\t";
    for(uint32_t i = 0; i < id.size(); ++i) std::cout << i << ' ';
    std::cout << "\nid[index]:\t";
    std::copy(id.cbegin(), id.cend(), id_out);
}

template <unsigned int sz>
void UnionFind<sz>::reset() {
    for(size_type i = 0; i < sz; ++i) id[i] = i;
}
#endif // UNIONFIND_HPP_INCLUDED
