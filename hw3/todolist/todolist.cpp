#include <stdexcept>
#include "todolist.h"
using namespace std;


DailyTodoList::DailyTodoList()
{
    data_ = new Item * [5];
    cap_ = 5;
    priorityHead_ = NULL;
    // [TO BE COMPLETED]
    //===========================
    // Add any addition code here
    sizePriorityList = 0;
    priorityEnd_ = NULL;
}

DailyTodoList::~DailyTodoList()
{
  for (int i = 0; i < cap_; i++)  //removes each todolist item
  {
    while(!empty(i))
      remove(i,0);
  }
  delete [] data_;
}

void DailyTodoList::push_back(size_t dayIndex, const std::string& val, bool highPriority)
{
    if (dayIndex < cap_)
    {
        this->insert(dayIndex, this->numItemsOnDay(dayIndex), val, highPriority);
    }
    else
    {
        this->insert(dayIndex, 0, val, highPriority);
    }

}

void DailyTodoList::insert(size_t dayIndex, size_t loc, const std::string& val, bool highPriority)
{
  //will resize if loc == 0 & if dayIndex >= cap_
  if (dayIndex >= cap_)
    if (loc == 0)
      resize(dayIndex);
    else
      throw std::out_of_range("insert loc is out of range");
  //will throw an error if loc > numItemsOnDay
  if (loc > numItemsOnDay(dayIndex))
    throw std::out_of_range("insert loc is out of range");
  //creates item to be inserted
  Item* insertion = new Item(val, NULL, NULL);
  //inserts into high priority list as appropriate
  if (highPriority)
  {
    sizePriorityList++;
    if (priorityEnd_ == NULL)
    {
      priorityEnd_ = insertion;
      priorityHead_ = insertion;
    }
    else {
      priorityEnd_->nextPriorityItem = insertion;
      priorityEnd_ = insertion;
    }
  }

  Item* header = data_[dayIndex];
  size_t counter = 0;
  if (loc == 0) //for if inserting at the beginning of a list
  {
    insertion->nextItem = header;
    data_[dayIndex] = insertion;
  }
  else
  {
    while (counter+1 < loc) //finds the item before the insertion spot
    {
      header = header->nextItem;
      counter++;
    }
    Item* temp = header->nextItem;
    header->nextItem = insertion;
    insertion->nextItem = temp;
  }

  return;
}

void DailyTodoList::resize(size_t orig_size)
{
  //doubles the size of the array
  Item** dataTemp_ = data_;
  cap_ = orig_size*2;
  data_ = new Item * [cap_];
  for (int i = 0; i < orig_size; i++)
    data_[i] = dataTemp_[i];
  for (int i = orig_size; i < cap_; i++)
    data_[i] = NULL;
  //deallocates previously-used pointers
  delete [] dataTemp_;
}

void DailyTodoList::remove(size_t dayIndex, size_t loc)
{
  //throws errors if the dayIndex/loc are invalid
  if (loc > numItemsOnDay(dayIndex))
    throw std::out_of_range("insert loc is out of range");
  if (dayIndex >= cap_)
    throw std::out_of_range("Remove dayIndex is out of range");

  Item* header = data_[dayIndex];
  Item* temp = NULL;
  //takes the desired item out of the array
  if (loc == 0)
  {
    data_[dayIndex] = header->nextItem;
    temp = header;
  }
  else
  {
    int counter = 0;
    while (counter+1 < loc)
    {
      header = header->nextItem;
      counter++;
    }
    temp = header->nextItem;
    header->nextItem = temp->nextItem;
  }
  //takes the desire item out of the priority list
  if (temp->nextPriorityItem != NULL || temp == priorityEnd_)
  {
    sizePriorityList--;
    Item* priorityParse = priorityHead_;
    if (priorityParse == temp)
    {
      priorityParse = priorityParse->nextPriorityItem;
    }
    else
    {
      while (priorityParse->nextPriorityItem != temp)
      {
        priorityParse = priorityParse->nextPriorityItem;
      }
      priorityParse->nextPriorityItem = priorityParse->nextPriorityItem->nextPriorityItem;
    }
  }
  //deletes item
  delete temp;
}

size_t DailyTodoList::numDays() const
{
  return cap_;
}

size_t DailyTodoList::numItemsOnDay(size_t dayIndex) const
{
  if (empty(dayIndex))
    return 0;
  size_t counter = 0;
  Item* header = data_[dayIndex];
  while (header != NULL)  //traverses the list, counting
  {
    header = header->nextItem;
    counter++;
  }
  return counter;
}

bool DailyTodoList::empty(size_t dayIndex) const
{
  if (dayIndex >= cap_)
    throw std::invalid_argument("dayIndex is invalid");
  if (data_[dayIndex] == NULL)  //checks if the first element exists
    return true;
  else
    return false;
}

const std::string& DailyTodoList::getNormalVal(size_t dayIndex, size_t loc) const
{
  if (empty(dayIndex))
    throw std::invalid_argument("dayIndex is invalid");

  size_t counter = 0;
  Item* header = data_[dayIndex];
  while (counter != loc && header != NULL)  //traverses to loc
  {
    header = header->nextItem;
    counter++;
  }
  if (header == NULL)
    throw std::invalid_argument("no item at the specified location");
  return header->val; //returns val at loc
}
std::string& DailyTodoList::getNormalVal(size_t dayIndex, size_t loc)
{
  if (empty(dayIndex))
    throw std::invalid_argument("dayIndex is invalid");

  size_t counter = 0;
  Item* header = data_[dayIndex];
  while (counter != loc && header != NULL)
  {
    header = header->nextItem;
    counter++;
  }
  if (header == NULL)
    throw std::invalid_argument("no item at the specified location");
  return header->val;
}

size_t DailyTodoList::numPriorityItems() const
{
  return sizePriorityList;
}

const std::string& DailyTodoList::getPriorityVal(size_t priorityLoc) const
{
  size_t counter = 0;
  if (priorityLoc >= sizePriorityList)
    throw std::out_of_range("no item at specified priority location");
  Item* priorityHeader = priorityHead_;

  if (sizePriorityList == 1)
    {
      return priorityHeader->val;
    }
  else
  {
    while (counter != priorityLoc && priorityHeader != NULL)  //traverses list
    {
      priorityHeader = priorityHeader->nextPriorityItem;
      counter++;
    }
    if (priorityHeader == NULL)
      throw std::out_of_range("no item at specified priority location");
    return priorityHeader->val; //returns item's value
  }
}
