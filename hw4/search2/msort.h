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
void msort(std::vector<T>& list, std::vector<T>& output, int start, int end, Compare comp)
{
  if (end-start <= 1)
    return;
  int mid = (start+end)/2;
  msort(list, output, start, mid, comp);
  msort(list, output, mid, end, comp);
  merge(list, output, start, mid, mid, end, comp);
  //necessary bookkeeping to update list?
}

//will merge two lists
template <class T, class Compare>
void merge(std::vector<T>& list, std::vector<T>& output, int start, int middle1, int middle2, int end, Compare comp)
{
  int outputCounter = start;
  while (middle2 < end)
  {
    while (comp(list[start], list[middle2]) && start < middle1)
    {
      output[outputCounter] = list[start];
      start++;
      outputCounter++;
    }
    //swap left and middle2
    output[outputCounter] = list[middle2];
    middle2++;
    outputCounter++;
  }
  while (start < middle1)
  {
    output[outputCounter] = list[start];
    outputCounter++;
    start++;
  }
  for (int i = 0; i < list.size(); i++)
  {
    list[i] = output[i];
  }
}
