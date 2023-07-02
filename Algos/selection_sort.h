#pragma once

#include <functional>
#include <algorithm>

using std::swap;

template <class RAC, typename Less = std::less<typename RAC::value_type>> //Random Access Compliant Container
RAC selection_sort(const RAC &container, Less compare = Less())
{
    const size_t N = container.size();
    RAC sorted(container);

    for(size_t i = 0; i < N - 1; ++i)
    {
      size_t mini = i;
      for(decltype(i) j = i + 1; j < N; ++j)
        if(compare(sorted[j], sorted[mini]))
          mini = j;
      swap(sorted[i], sorted[mini]);
    }
    return sorted;
}

template <typename T, unsigned S>
auto selection_sort(T (&arr)[S]) -> decltype(arr)
{
    const size_t N = S;
    // or size_t N = sizeof(arr) / sizeof(T);
    T (&sorted)[S] = arr;
    //std::copy(std::begin(arr), std::end(arr), std::begin(sorted));
    //for (int i = 0; i < N; ++i) sorted[i] = arr[i];

    for(size_t i = 0; i < N - 1; ++i)
    {
      size_t mini = i;
      for(decltype(i) j = i + 1; j < N; ++j)
        if(sorted[j] < sorted[mini])
          mini = j;
      swap(sorted[i], sorted[mini]);
    }
    return sorted;
}
