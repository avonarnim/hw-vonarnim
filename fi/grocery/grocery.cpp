//
// You must provide your implementation of maxGroceryBox() and all
// helper functions in this file.
//

#include "grocery.h"

// You may prototype helper functions here and implement them below


std::vector<GItem> maxGroceryBox(const std::vector<GItem>& inventory, int weight_limit)
{
    // TODO implement!
    std::vector<GItem> results;
    return constructBox(inventory, results, weight_limit, 0);

}

std::vector<GItem> constructBox(std::vector<GItem> inventory, std::vector<GItem> holdings, unsigned int weight_limit, int idx)
{
  if (idx == inventory.size())
    return holdings;
  std::vector<GItem> resultWO = constructBox(inventory, holdings, weight_limit, idx + 1);
  holdings.push_back(inventory[idx]);
  if (getWeight(holdings) > weight_limit)
  {
    holdings.pop_back();
  }
  std::vector<GItem> resultW = constructBox(inventory, holdings, weight_limit, idx + 1);
  if (getValue(resultWO) > getValue(resultW)) //return max of two options
    return resultWO;
  else
    return resultW;
}


int getWeight(const std::vector<GItem> box)
{
  int totalW = 0;
  for (int i = 0; i < box.size(); i++)
  {
    totalW += box[i].weight;
  }
  return totalW;
}

int getValue(const std::vector<GItem> box)
{
  int value = 0;
  for (int i = 0; i < box.size(); i++)
  {
    value += box[i].value;
  }
  return value;
}
