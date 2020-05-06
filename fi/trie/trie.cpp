//
// Students: write your trie implementation below.
//

#include "trie.h"

//----------------------------------------------
// DO NOT MODIFY THE CONSTRUCTOR OR DESTRUCTOR.
//----------------------------------------------

SparseTrie::SparseTrie()
{
    // DO NOT MODIFY.
    // The root node is always present and represents an
    // empty string. It should never be removed until
    // the destructor runs.
    root = new TNode('\0', false);
}

SparseTrie::~SparseTrie()
{
    // LEAVE BLANK. Don't worry about deallocation.
}

void SparseTrie::insert(std::string key)
{
  TNode* current = root->children, prev = root;
  int index = 0;
  while (key[index] != '\0' && current != NULL)
  {
    prev = current;
    current = current->children; //set current to appropriate child
    while (current != NULL && current->letter != key[index])
      current = current->next;
    index++;
  }
  if (current)
  {
    node->terminal = true;
  }
  else
  {
    while (key[index] != '\0')
    {
      current = new TNode(key[index], false)
      prev->children = current;
      prev = current;
      index++;
    }
    prev->terminal = true;
    if (root == NULL)
    {
      root->children = current;
    }
  }
}

bool SparseTrie::contains(std::string key)
{
  int index = 0;
  TNode* current = root;
  return (containsHelper(key, current, index));

}

bool SparseTrie::containsHelper(std::string key, TNode* current, int index)
{
  if (index == key.length()-1)
  {
    bool result = false;
    while (current != NULL)
    {
      if (current->letter == key[key.length()-1] && terminal)
        result = true;
    }
    return result;
  }
  TNode* childTraversal = current->children;
  while (childTraversal != NULL)
  {
    if (childTraversal->letter == key[index])
    {
      return containsHelper(key, childTraversal, index + 1);
    }
    childTraversal = childTraversal->next;
  }
  return false;
}
