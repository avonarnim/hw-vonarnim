#ifndef HEAP_H
#define HEAP_H
#include <functional>
#include <stdexcept>
#include <vector>

template <typename T, typename PComparator = std::less<T> >
class Heap
{
public:
    /// Constructs an m-ary heap for any m >= 2
    Heap(int m, PComparator c = PComparator());

    /// Destructor as needed
    ~Heap();

    /// Adds an item
    void push(const T& item);
    void trickleUp(int loc);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();
    void heapify(int idx);

    /// returns true if the heap is empty
    bool empty() const;

private:
    /// Add whatever helper functions and data members you need below
    std::vector<T> items;
    int childrenSize;
    PComparator comp;

};

// Add implementation of member functions here

template <typename T, typename PComparator>
Heap<T,PComparator>::Heap(int m, PComparator c)
{
  T placeHolder;
  items.push_back(placeHolder);  //i'd like to insert a 0/null for ease of indexing...
  childrenSize = m;
  comp = c;
}

template <typename T, typename PComparator>
Heap<T,PComparator>::~Heap()
{

}

template <typename T, typename PComparator>
void Heap<T,PComparator>::push(const T& item)
{
  items.push_back(item);
  trickleUp(items.size()-1);
}

template <typename T, typename PComparator>
void Heap<T,PComparator>::trickleUp(int loc)
{
  int parent = loc/2;
  while (parent >= 1 && comp(items[loc], items[parent]))
  {
    //swapping
    T temp = items[parent];
    items[parent] = items[loc];
    items[loc] = temp;
    loc = parent;
    parent = loc/2;
  }
}

template <typename T, typename PComparator>
bool Heap<T,PComparator>::empty() const
{
  if (items.size() <= 1)
    return true;
  else
    return false;
}


// We will start top() for you to handle the case of
// calling top on an empty heap
template <typename T, typename PComparator>
T const & Heap<T,PComparator>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }
    // If we get here we know the heap has at least 1 item
    // Add code to return the top element
    return items[1];
}


// We will start pop() for you to handle the case of
// calling top on an empty heap
template <typename T, typename PComparator>
void Heap<T,PComparator>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    items[1] = items.back();
    items.pop_back();
    heapify(1);
}

template <typename T, typename PComparator>
void Heap<T,PComparator>::heapify(int idx)
{
  if (idx > items.size()/2)
    return;
  int smallerChild = 2*idx;
  if (smallerChild+1 < items.size())
  {
    int rightChild = smallerChild+1;
    if (comp(items[rightChild], items[smallerChild]))
      smallerChild = rightChild;
  }
  if (comp(items[smallerChild], items[idx]))
    {
      T temp = items[idx];
      items[idx] = items[smallerChild];
      items[smallerChild] = temp;
      heapify(smallerChild);
    }
}


#endif
