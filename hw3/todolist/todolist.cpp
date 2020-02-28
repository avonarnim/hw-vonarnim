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
  for (int i = 0; i < cap_; i++)
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

/**
 *  @param[in] dayIndex
 *    Day index of the to-do list that val should be inserted into.
 *  @param[in] loc
 *    Location to insert the new value in the given day's to-do list
 *  @param[in] val
 *    String value to insert
 *  @param[in] highPriority
 *    True if this value should also be added to the end of the high priority list
 *  @throws std::out_of_range - If the location is beyond the end of the given to-do list.
 *  @throws std::bad_alloc -  Memory could not be allocated
 */
void DailyTodoList::insert(size_t dayIndex, size_t loc, const std::string& val, bool highPriority)
{
  Item* insertion = new Item(val, NULL, NULL);
  Item* header = data_[dayIndex];
  size_t counter = 0;
  if (highPriority)
  {
    if (priorityEnd_ == NULL)
      priorityEnd_ = insertion;
    else {
      priorityEnd_->nextPriorityItem = insertion;
      priorityEnd_ = insertion;
    }
  }
  if (loc == 0)
  {
    resize(dayIndex);
    header = insertion;
    insertion->nextItem = NULL;
    return;
  }

  while (counter+1 < loc)
  {
    header = header->nextItem;
    counter++;
  }
  Item* temp = header->nextItem;
  header->nextItem = insertion;
  insertion->nextItem = temp;
  return;
}

void DailyTodoList::resize(size_t orig_size)
{
  Item** dataTemp_ = data_;
  data_ = new Item * [orig_size*2];
  for (int i = 0; i < orig_size; i++)
    delete [] dataTemp_[i];
  delete [] dataTemp_;
}


/*
 *  @throws std::out_of_range
 *    If the location is invalid.
 */
void DailyTodoList::remove(size_t dayIndex, size_t loc)
{
  if (dayIndex > cap_)
    throw std::out_of_range("Remove dayIndex is out of range");
  Item* header = data_[dayIndex];
  int counter = 0;
  while (counter+1 < loc && header != NULL)
  {
    header = header->nextItem;
    counter++;
  }
  if (header == NULL)
    throw std::out_of_range("Remove loc is out of range");
  Item* temp = header->nextItem;
  header->nextItem = temp->nextItem;
  if (temp->nextPriorityItem != NULL)
  {
    Item* priorityParse = priorityHead_;
    while (priorityParse->nextPriorityItem != header)
    {
      priorityParse = priorityParse->nextPriorityItem;
    }
    priorityParse->nextPriorityItem = priorityParse->nextPriorityItem->nextPriorityItem;
  }
  delete [] temp;
}

/**
 *  Returns the number of days / to-do lists stored
 *  Days range from 0 to n-1 and represent the size/capacity
 *  of the array storing the to-do lists for each day.
 */
size_t DailyTodoList::numDays() const
{
  return cap_;
}

/**
 *  Returns the number of items in the to-do list for the given day
 *
 *  @param[in] dayIndex
 *    Index of the day to return the number of to-do list items
 *  @throws std::out_of_range
 *    If the dayIndex is invalid.
 */
size_t DailyTodoList::numItemsOnDay(size_t dayIndex) const
{
  if (empty(dayIndex))
    return 0;
  size_t counter = 0;
  Item* header = data_[dayIndex];
  while (header != NULL)
  {
    header = header->nextItem;
    counter++;
  }
  return counter;
}

/**
 *  Returns true if the to-do list for the given day is empty
 *
 *  @param[in] dayIndex
 *    Index of the desired to-do list
 *  @returns true if the to-do list for the given day is empty,
 *     and false otherwise.
 *  @throws std::out_of_range
 *    If the dayIndex is invalid.
 */
bool DailyTodoList::empty(size_t dayIndex) const
{
  if (dayIndex > cap_)
    throw std::invalid_argument("dayIndex is invalid");
  if (data_[dayIndex]->nextItem == NULL)
    return true;
  else
    return false;
}

/**
 *  Returns the to-do value for the given day and location in that
 *  day's to-do list.
 *
 *  @param[in] dayIndex
 *    Index of the day's to-do list
 *  @param[in] loc
 *    Location of the item to get from the given day's to-do list
 *  @returns the to-do value
 *  @throws std::out_of_range
 *    If the dayIndex is invalid.
 */
const std::string& DailyTodoList::getNormalVal(size_t dayIndex, size_t loc) const
{
  if (empty(dayIndex))
    throw std::invalid_argument("dayIndex is invalid");
  if (dayIndex > cap_)
    throw std::invalid_argument("dayIndex is invalid");

  size_t counter = 0;
  Item* header = data_[dayIndex];
  while (counter != loc)  // should probably do ""&& header != NULL" to check for loc > size of day to do list
  {
    header = header->nextItem;
    counter++;
  }
  return header->val;
}
std::string& DailyTodoList::getNormalVal(size_t dayIndex, size_t loc)
{
  if (empty(dayIndex))
    throw std::invalid_argument("dayIndex is invalid");
  if (dayIndex > cap_)
    throw std::invalid_argument("dayIndex is invalid");

  size_t counter = 0;
  Item* header = data_[dayIndex];
  while (counter != loc)  // should probably do ""&& header != NULL" to check for loc > size of day to do list
  {
    header = header->nextItem;
    counter++;
  }
  return header->val;
}

/**
 *  Returns the number of values in the high priority list
 */
size_t DailyTodoList::numPriorityItems() const
{
  return sizePriorityList;
}

/**
 *  Returns the to-do value for the given location in the high
 *  priority list.
 *
 *  @param[in] priorityLoc
 *    Location of the item to get from the high priority list
 *  @returns the to-do value
 *  @throws std::out_of_range
 *    If priorityLoc is invalid.
 */
const std::string& DailyTodoList::getPriorityVal(size_t priorityLoc) const
{
  size_t counter = 0;
  Item* priorityHeader = priorityHead_;
  while (counter != priorityLoc)  // should probably do ""&& header != NULL" to check for loc > size of day to do list
  {
    priorityHeader = priorityHeader->nextPriorityItem;
    counter++;
  }
  return priorityHeader->val;
}
