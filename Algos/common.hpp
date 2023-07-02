#pragma once
#include <functional>
#include <type_traits>
#include <cassert>

#define ws ' '

namespace algo
{
namespace sorts
{

using std::extent;
using std::swap;

template <class C>
bool is_sorted(const C &container, unsigned b, unsigned e)
{
  return std::is_sorted(std::begin(container) + b, std::begin(container) + e);
}


} //sorts

} //algo