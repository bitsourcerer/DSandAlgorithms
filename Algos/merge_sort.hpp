#ifndef MERGE_SORT_HPP_INCLUDED
#define MERGE_SORT_HPP_INCLUDED

#include <cassert>
#include <algorithm>
#include <functional>
#include "common.hpp"

//#define USE_INSERTION_SORT

#if defined(USE_INSERTION_SORT)
  #include "insertion_sort.h"
  #define CUTOFF 7
#else
  #define CUTOFF 0
#endif // USE_INSERTION_SORT

namespace algo
{
namespace sorts
{

enum class MergeType
{
  CLASSIC,
  BOTTOM_UP
};


/// That instead of unsigned we can use iterator for hi and lo. so mid shall be an iterator too.
template <class RAC, typename Predicate = std::less<typename RAC::value_type>>
void merge_sect(RAC &con, RAC &aux, unsigned lo, unsigned mid, unsigned hi, Predicate compare = Predicate())
{
  // assert(is_sorted(con, lo, mid));
  // assert(is_sorted(con, mid+1, hi));
  /// Takes linear N time to traverse each array or container which is expensive. also need to be in #ifndef USE_INSERTION_SORT

  static const size_t N = con.size();
  for(unsigned k = lo; k <= hi; ++k) aux[k] = con[k];

  unsigned i = lo, j = mid + 1;
  for(unsigned k = lo; k <= hi; ++k)
  {
    if(i > mid) con[k] = aux[j++];
    else if(j > hi) con[k] = aux[i++];
    else if(compare(aux[j], aux[i])) con[k] = aux[j++];
    else con[k] = aux[i++];
  }
  #ifndef USE_INSERTION_SORT
    assert(is_sorted(con, lo, hi));
  #endif
}

template <class RAC, typename Predicate = std::less<typename RAC::value_type>>
void sort_sect(RAC &con, RAC &aux, unsigned lo, unsigned hi, Predicate compare = Predicate())
{
  #ifdef USE_INSERTION_SORT
    if(hi <= lo + CUTOFF) { con = insertion_sort(con); return; }
  #endif
  if(hi <= lo) return;
  unsigned mid = lo + (hi - lo) / 2;

  sort_sect(con, aux, lo, mid, compare); // start with the wholeleft side and keep splitting it untill we are done
  sort_sect(con, aux, mid+1, hi, compare); // then move to the right side of the container and split it untill the end too.
  if(!compare(con[mid+1], con[mid])) return;
  merge_sect<RAC>(con, aux, lo, mid, hi, compare);
}

template <class RAC, typename Predicate = std::less<typename RAC::value_type>>
RAC& merge_sort(RAC &con, Predicate compare = Predicate())
{
  RAC aux(con.size());
  sort_sect(con, aux, 0, con.size() - 1);
  return con;
}

//____________________________________________________ARRAYS_________________________________________________________________//

template<typename T, unsigned S> void merge_sect(T (&con)[S], T (&aux)[S], unsigned lo, unsigned mid, unsigned hi)
{
  for(unsigned k = lo; k <= hi; ++k) aux[k] = con[k];

  unsigned i = lo, j = mid + 1;
  for(unsigned k = lo; k <= hi; ++k)
  {
    if(i > mid) con[k] = aux[j++];
    else if(j > hi) con[k] = aux[i++];
    else if(aux[j] < aux[i]) con[k] = aux[j++];
    else con[k] = aux[i++];
  }
  // assert(is_sorted(con, lo, hi));
}

template <typename T, unsigned S>
void sort_sect(T (&con)[S], T (&aux)[S], unsigned lo, unsigned hi)
{
  #ifdef USE_INSERTION_SORT
    if(hi <= lo + CUTOFF - 1) { insertion_sort(con); return; }
  #else
    if(hi <= lo) return;
  #endif

  unsigned mid = lo + (hi - lo) / 2;

  sort_sect(con, aux, lo, mid); // start with the wholeleft side and keep splitting it untill we are done
  sort_sect(con, aux, mid+1, hi); // then move to the right side of the container and split it untill the end too.
  merge_sect(con, aux, lo, mid, hi);
}

template <typename T, unsigned S>
auto merge_sort(T (&con)[S]) -> decltype(con)
{
  T aux[S];
  sort_sect(con, aux, 0, S - 1);
  return con;
}


} // sorts

} // algo


// if(i > mid) aux[k] = con[j++];
    // if(j > hi) aux[k] = con[i++];
    // if(compare(con[j], con[i])) aux[k] = con[j++];
    // else aux[k] = con[i++];

#endif // MERGE_SORT_HPP_INCLUDED
