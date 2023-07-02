#ifndef INSERTION_SORT_H_INCLUDED
#define INSERTION_SORT_H_INCLUDED

template <class RAC, typename Less = std::less<typename RAC::value_type>> //Random Access Compliant Container
RAC insertion_sort(const RAC &container, Less compare = Less())
{
    const size_t N = container.size();
    RAC sorted(container);

    for(size_t i = 1; i < N; ++i)
    {
      for(decltype(i) j = i; j > 0; --j)
        if(compare(sorted[j], sorted[j-1]))
            std::swap(sorted[j-1], sorted[j]);
        else break;
    }
    return sorted;
}

template <typename T, unsigned S>
auto insertion_sort(T (&arr)[S]) -> decltype(arr)
{
    const size_t N = S;
    T (&sorted)[S] = arr; //refrence to array passed as arg

    for(size_t i = 0; i < N; ++i)
    {
      for(decltype(i) j = i; j > 0; --j)
        if(sorted[j] < sorted[j-1]) //if the current number is smaller than the one behind it then swap it
          // after swapping move to it again and repeat the steps.
          std::swap(sorted[j-1], sorted[j]);
        else break;
    }
    return sorted;
}

#endif // INSERTION_SORT_H_INCLUDED
