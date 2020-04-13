#include "expr-reader.h"
#include "expr-ops.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;
/*-----------------------------------------------------------------------------
 *  Implement the following functions below.
 *  Processing of each (sub)expression must be recursive (i.e. loops
 *    may only be used to iterate through the child vector of the given node).
 *  You may define recursive helper functions to adapt the function signatures.
 *    However, only exprPrinter likely needs a helper function.
 ----------------------------------------------------------------------------*/

void exprPrinter(std::ostream& ostr, Node* n)
{
  if (n->children.size() != 0)
  {
    ostr << "( ";
    for (unsigned int i = 0; i < n->children.size()-1; i++)
    {
      exprPrinter(ostr, n->children[i]);
      ostr << n->val << " ";
    }
    exprPrinter(ostr, n->children[n->children.size()-1]);
    ostr << ") ";
  }
  if (n->children.size() == 0)
  {
    ostr << n->val << " ";
  }
}


int exprSolver(Node* n)
{
    /* Should never be called on NULL expression -- Do Not Modify */
    if(n == NULL ) {
        throw std::logic_error("Can't solve a NULL expression");
    }
    vector<int> additors;
    for (unsigned int i = 0; i < n->children.size(); i++)
    {
      additors.push_back(exprSolver(n->children[i]));
    }
    if (n->children.size() == 0)
      return std::stoi(n->val);
    else
    {
      int retVal = additors[0];
      for (unsigned int i = 1; i < additors.size(); i++)
      {
        if (n->val == "+")
          retVal = retVal + additors[i];
        else if (n->val == "-")
          retVal = retVal - additors[i];
        else if (n->val == "*")
          retVal = retVal * additors[i];
        else if (n->val == "/")
          retVal = retVal / additors[i];
      }
      return retVal;
    }
}

// checks if all leaf nodes are strings convertible to integers
bool exprIsValid(Node* n)
{
  if(n == NULL ) {
      throw std::logic_error("Can't solve a NULL expression");
  }
  if (n->children.size() == 0)
  {
    string value = n->val;
    std::stringstream ss(value);
    int number;
    if (ss >> number)
      return true;
    else
      return false;
  }
  bool answers = true;
  for (unsigned int i = 0; i < n->children.size(); i++)
  {
    if (!exprIsValid(n->children[i]))
      answers = false;
  }
  return answers;
}
