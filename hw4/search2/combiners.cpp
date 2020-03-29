#include "searcheng.h"
#include "combiners.h"

// Place your function implementations for your derived
// WebPageSetCombiner classes here
WebPageSet ANDWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
{
  WebPageSet returnSet;
  if (setA.empty())
    return returnSet;
  std::set<WebPage*>::iterator it;
  WebPage* transfer;
  for (it = setB.begin(); it != setB.end(); ++it)
  {
    transfer = *it;
    if (setA.find(transfer) != setA.end())
      returnSet.insert(transfer);
  }
  return returnSet;
}

WebPageSet DiffWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
{
  WebPageSet returnSet;
  if (setA.empty())
    return returnSet;
  std::set<WebPage*>::iterator it;
  WebPage* transfer;
  for (it = setA.begin(); it != setA.end(); ++it)
  {
    transfer = *it;
    if (setB.find(transfer) == setB.end())
      returnSet.insert(transfer);
  }
  return returnSet;
}

WebPageSet ORWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
{
  WebPageSet returnSet;
  std::set<WebPage*>::iterator it;
  WebPage* transfer;
  for (it = setA.begin(); it != setA.end(); ++it)
  {
    transfer = *it;
    returnSet.insert(transfer);
  }
  for (it = setB.begin(); it != setB.end(); ++it)
  {
    transfer = *it;
    if (returnSet.find(transfer) == returnSet.end())
      returnSet.insert(transfer);
  }
  return returnSet;
}
