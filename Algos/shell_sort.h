#ifndef SHELL_SORT_H_INCLUDED
#define SHELL_SORT_H_INCLUDED

template <class RAC, typename Less = std::less<typename RAC::value_type>> //Random Access Compliant Container
RAC shell_sort(const RAC &container, Less compare = Less())
{
    const size_t N = container.size();
    size_t h = 1;
    while(h <= N/3) h = h * 3 + 1;  //set h to the maximum under the size of container
    RAC sorted(container);

    while(h >= 1)
    {
      for(size_t i = h; i < N; ++i)
      {
        for(decltype(i) j = i; j >= h && compare(sorted[j], sorted[j-h]); j -= h)
            std::swap(sorted[j-h], sorted[j]);
      }
      h /= 3;
    }
    return sorted;
}

template <typename T, unsigned S>
auto shell_sort(T (&arr)[S]) -> decltype(arr)
{
    const size_t N = S;
    T (&sorted)[S] = arr; //refrence to array passed as arg

    size_t h = 1;
    while(h <= N/3) h = h * 3 + 1;

    while(h > 0)
    {
      for(size_t i = h; i < N; ++i)
      {
        for(decltype(i) j = i; j >= h && sorted[j] < sorted[j-h]; j -= h)
            std::swap(sorted[j-h], sorted[j]);
      }
      h = h / 3;
    }
    return sorted;
}

#endif // SHELL_SORT_H_INCLUDED
