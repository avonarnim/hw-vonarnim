#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    char getBalance () const;
    void setBalance (char balance);
    void updateBalance(char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    char balance_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateRight( AVLNode<Key, Value>* n);
    void rotateLeft( AVLNode<Key, Value>* n);
    bool isLeftChild( AVLNode<Key, Value>* n,  AVLNode<Key, Value>* p);
    bool isRightChild( AVLNode<Key, Value>* n,  AVLNode<Key, Value>* p);
    void insertBST(const std::pair<const Key, Value> &new_item);
    void removeBST(const Key& key);
    void insertFix(AVLNode<Key, Value>* parent,  AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* n,  char diff);

};

/*
 * Recall: If key is already in the tree, you should
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    insertBST(new_item);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertBST(const std::pair<const Key, Value> &new_item)
{
  Node<Key, Value>* duplicate = BinarySearchTree<Key,Value>::internalFind(new_item.first);
  if (duplicate != NULL)
  {
    duplicate->getValue() = new_item.second;
    return;
  }
  std::cout << "inserting " << new_item.first << std::endl;
  AVLNode<Key, Value>* insertion = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
  insertion->setRight(NULL);
  insertion->setLeft(NULL);

  Node<Key, Value>* temp = BinarySearchTree<Key, Value>::root_; //returns node type
  while (temp != NULL)
  {
    insertion->setParent(temp);
    if (temp->getKey() > new_item.first)
    { temp = temp->getLeft(); }
    else if (temp->getKey() < new_item.first)
    { temp = temp->getRight(); }
  }

  if (insertion->getParent() == NULL)
  {
    BinarySearchTree<Key, Value>::root_ = insertion;  //balance is already 0
    return;
  }
  else if (new_item.first > insertion->getParent()->getKey())
    { insertion->getParent()->setRight(insertion); }
  else
    { insertion->getParent()->setLeft(insertion); }

  AVLNode<Key, Value>* helpPointer = insertion->getParent();
  if (helpPointer->getBalance() == -1)
    { helpPointer->setBalance(0); }
  else if (helpPointer->getBalance() == 1)
    { helpPointer->setBalance(0); }
  else if (helpPointer->getRight() == insertion)
  {
    helpPointer->setBalance(1);
    insertFix(helpPointer, insertion);
  }
  else
  {
    helpPointer->setBalance(-1);
    insertFix(helpPointer, insertion);
  }
  return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent,  AVLNode<Key, Value>* node)
{
  std::cout << "in insertFix" << std::endl;
  BinarySearchTree<Key, Value>::print();
  if (parent == NULL || parent->getParent() == NULL)  //evaluate the properness of this.
    return;
  AVLNode<Key, Value>* grand = parent->getParent();
  if (isLeftChild(parent, grand))
  {
    grand->updateBalance(-1);
    if (grand->getBalance() == 0)
      return;
    else if (grand->getBalance() == -1)
      {
        std::cout << "fixing grandparent" << std::endl;
        BinarySearchTree<Key, Value>::print();
        insertFix(grand, parent);
      }
    else if (grand->getBalance() == -2)
    {
      if (isLeftChild(node, parent))
      {
        std::cout << "before rotating right" << std::endl;
        BinarySearchTree<Key, Value>::print();
        rotateRight(grand);
        parent->setBalance(0);
        grand->setBalance(0);
      }
      else
      {
        std::cout << "before rotating left, then right" << std::endl;
        BinarySearchTree<Key, Value>::print();
        rotateLeft(parent);
        rotateRight(grand);
        if (node->getBalance() == -1)
        {
          parent->setBalance(1);
          node->setBalance(0);
        }
        else if (node->getBalance() == 0)
        {
          parent->setBalance(0);
          node->setBalance(0);
          grand->setBalance(0);
        }
      }
    }
    std::cout << "done with insertFix" << std::endl;
    BinarySearchTree<Key, Value>::print();
  }
  else
  {
    grand->updateBalance(1);
    if (grand->getBalance() == 0)
      return;
    else if (grand->getBalance() == 1)
      {
        std::cout << "before fixing grandparent" << std::endl;
        BinarySearchTree<Key, Value>::print();
        insertFix(grand, parent);
      }
    else if (grand->getBalance() == 2)
    {
      if (isRightChild(node, parent))
      {
        std::cout << "before rotating left" << std::endl;
        BinarySearchTree<Key, Value>::print();
        rotateLeft(grand);
        parent->setBalance(0);
        grand->setBalance(0);
      }
      else
      {
        std::cout << "before rotating right then left" << std::endl;
        BinarySearchTree<Key, Value>::print();
        rotateRight(parent);
        rotateLeft(grand);
        if (node->getBalance() == 1)
        {
          parent->setBalance(-1);
          node->setBalance(0);
        }
        else if (node->getBalance() == 0)
        {
          parent->setBalance(0);
          node->setBalance(0);
          grand->setBalance(0);
        }
      }
    }
    std::cout << "done with insert fix" << std::endl;
    BinarySearchTree<Key, Value>::print();
  }
}
/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    removeBST(key);
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeBST(const Key& key)
{
  Node<Key, Value>* searched = BinarySearchTree<Key, Value>::internalFind(key);
  AVLNode<Key, Value>* searchFor = (AVLNode<Key, Value>*) searched;
  //BinarySearchTree<Key, Value>::iterator it(searchFor);
  if (searchFor == NULL)
    return;
  else
  {
    int diff = 0;
    if (searchFor->getLeft() != NULL && searchFor->getRight() != NULL)
    {
      nodeSwap(searchFor, (AVLNode<Key, Value>*) BinarySearchTree<Key, Value>::predecessor(searchFor));
    }
    AVLNode<Key, Value>* p = searchFor->getParent();
    if (p != NULL)
    {
      if (isLeftChild(searchFor, p))
      {
        diff = 1;
        p->setLeft(NULL);
      }
      else
      {
        diff = -1;
        p->setRight(NULL);
      }
    }
    delete searchFor;
    removeFix(p, diff);
  }
}
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n,  char diff)
{
  if (n == NULL)
    return;
  AVLNode<Key, Value>* p = n->getParent();
  int ndiff;
  if (p != NULL)
  {
    if (isLeftChild(n, p))
      ndiff = 1;
    else
      { ndiff = -1; }
  }
  AVLNode<Key, Value>* c = NULL;
  if (diff == -1)
  {
    if (n->getBalance() + diff == -2)
    {
      c = n->getLeft();
      if (c->getBalance() == -1)
      {
        rotateRight(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
      }
      else if (c->getBalance() == 0)
      {
        rotateRight(n);
        n->setBalance(-1);
        c->setBalance(1);
      }
      else if (c->getBalance() == 1)
      {
        AVLNode<Key, Value>* g = c->getRight();
        rotateLeft(c);
        rotateRight(n);
        if (g->getBalance() == 1)
        {
          n->setBalance(0);
          c->setBalance(-1);
          g->setBalance(0);
        }
        else if (g->getBalance() == 0)
        {
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else if (g->getBalance() == -1)
        {
          n->setBalance(1);
          c->setBalance(0);
          g->setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    else if (n->getBalance() + diff == -1)
    {
      n->setBalance(-1);
    }
    else if (n->getBalance() + diff == 0)
    {
      n->setBalance(0);
      removeFix(p, ndiff);
    }
  }
  else
  {
    //diff == 1
    if (n->getBalance() + diff == 2)
    {
      c = n->getLeft();
      if (c->getBalance() == 1)
      {
        rotateRight(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
      }
      else if (c->getBalance() == 0)
      {
        rotateRight(n);
        n->setBalance(1);
        c->setBalance(-1);
      }
      else if (c->getBalance() == -1)
      {
        AVLNode<Key, Value>* g = c->getRight();
        rotateRight(c);
        rotateLeft(n);
        if (g->getBalance() == -1)
        {
          n->setBalance(0);
          c->setBalance(1);
          g->setBalance(0);
        }
        else if (g->getBalance() == 0)
        {
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else if (g->getBalance() == 1)
        {
          n->setBalance(-1);
          c->setBalance(0);
          g->setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    else if (n->getBalance() + diff == 1)
    {
      n->setBalance(1);
    }
    else if (n->getBalance() + diff == 0)
    {
      n->setBalance(0);
      removeFix(p, ndiff);
    }
  }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight( AVLNode<Key, Value>* n)
{
  //make this node the right child of the former left child
  Node<Key, Value>* temp = n->getLeft()->getRight();
  Node<Key, Value>* parent = n->getParent();
  if (parent == NULL)
  {
    BinarySearchTree<Key, Value>::root_ = n->getLeft();
  }
  else
  {
    if (isLeftChild(n, (AVLNode<Key, Value>*) parent))
    {
      parent->setLeft(n->getLeft());
    }
    else
    {
      parent->setRight(n->getLeft());
    }
  }
  n->getLeft()->setParent(parent);
  n->getLeft()->setRight(n);
  n->setParent(n->getLeft());
  n->setLeft(temp);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft( AVLNode<Key, Value>* n)
{
  //make this node the left child of the former right child
  Node<Key, Value>* temp = n->getRight()->getLeft();
  Node<Key, Value>* parent = n->getParent();
  if (parent == NULL)
  { //node was root_
    BinarySearchTree<Key, Value>::root_ = n->getRight();
  }
  else
  {
    if (isLeftChild(n, (AVLNode<Key, Value>*) parent))
    {
      parent->setLeft(n->getRight());
    }
    else
    {
      parent->setRight(n->getRight());
    }
  }
  n->getRight()->setParent(parent);
  n->getRight()->setLeft(n);
  n->setParent(n->getRight());
  n->setRight(temp);
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isLeftChild( AVLNode<Key, Value>* n,  AVLNode<Key, Value>* p)
{
  if (p->getLeft() == n)
    return true;
  else return false;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isRightChild( AVLNode<Key, Value>* n,  AVLNode<Key, Value>* p)
{
  if (p->getRight() == n)
    return true;
  else return false;
}


#endif
