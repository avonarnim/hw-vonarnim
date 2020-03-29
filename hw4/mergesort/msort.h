#include <vector>

/**
 * Performs mergesort on the provided vector using
 * the Compare object, comp, to peform ALL comparisons
 *
 * Must run in O(n*log(n))
 */
template <class T, class Compare >
void mergeSort(std::vector<T>& list, Compare comp );
template <class T, class Compare >
void msort(std::vector<T>& list, std::vector<T>& output, int left, int right, Compare comp);
template <class T, class Compare >
void merge(std::vector<T>& list, std::vector<T>& output, int left, int middle1, int middle2, int right, Compare comp);

template <class T, class Compare >
void mergeSort(std::vector<T>& list, Compare comp)
{
  std::vector<T> newList(list);  //copy of array
  msort(newList, list, 0, list.size(), comp);
}

template <class T, class Compare >
void msort(std::vector<T>& list, std::vector<T>& output, int left, int right, Compare comp)
{
  if (left >= right) return;
  int mid = (left+right)/2;
  msort(list, output, left, mid, comp);
  msort(list, output, mid, right, comp);
  merge(list, output, left, mid, mid, right, comp);
}

//will merge two lists
template <class T, class Compare >
void merge(std::vector<T>& list, std::vector<T>& output, int left, int middle1, int middle2, int right, Compare comp)
{
  while (middle2 < right)
  {
    while (comp(list[left], list[middle2]) && left < middle1)
    {
      output[left] = list[left];
      left++;
    }
    //swap left and middle2
    output[left] = list[middle2];
    middle2++;
  }
}
