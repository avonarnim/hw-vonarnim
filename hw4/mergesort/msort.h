#include <vector>

/**
 * Performs mergesort on the provided vector using
 * the Compare object, comp, to peform ALL comparisons
 *
 * Must run in O(n*log(n))
 */
template <class T, class Compare >
void mergeSort(std::vector<T>& list, Compare comp );
void msort(vector<T>& list, vector<T>& output, int left, int right, Compare comp);
void merge(std::vector<T>& list, vector<T>& output, int left, int middle, int middle, int right, Compare comp);

void mergeSort(std::vector<T>& list, Compare comp)
{
  vector<T> newList(list);  //copy of array
  msort(newList, list, 0, mlist.size());
}

void msort(vector<T>& list, vector<T>& output, int left, int right, Compare comp)
{
  if (left >= right) return;
  int mid = (left+right)/2;
  msort(list, output, left, mid);
  msort(list, output, mid, end);
  merge(list, output, left, mid, mid, right);
}

//will merge two lists
void merge(std::vector<T>& list, vector<T>& output, int left, int middle1, int middle2, int right, Compare comp)
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
