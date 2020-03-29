#include "llrec.h"

//*********************************************
// Provide your implementation of llpivot below
//*********************************************


/*
 linked list pointed to by head,
 create two lists
   values less than or equal to the pivot value
   values greater than or equal to the pivot
 The input list pointed to by head should be empty upon return
   head set to NULL (i.e. we are not making copies)
 */
void llpivot(Node *&head, Node *&smaller, Node *&larger, int pivot)
{
  if (head == NULL)
    return;
  Node* temp;
  if (head->val <= pivot)
  {
    temp = smaller;
    smaller = head;
    head = head->next;
    //below is a try-catch i'm not sure works. if works, get rid of smallEmpty variable.
    /*
    try {
    smaller->next = temp->next; }
    catch ("temp->next is garbage")
    {
      smaller->next = NULL;
    }
    */
    if (smallEmpty)
      smaller->next = NULL;
    else
      smaller->next = temp->next;
    smallEmpty = false;
  }
  else if (head->val > pivot)
  {
    temp = larger;
    larger = head;
    head = head->next;
    //below is a try-catch i'm not sure works. if works, get rid of largeEmpty variable.
    /*
    try {
    larger->next = temp->next; }
    catch ("temp->next is garbage")
    {
      larger->next = NULL;
    }
    */
    if (largeEmpty)
      larger->next = NULL;
    else
      larger->next = temp->next;
    largeEmpty = false;
  }
  llpivot(head->next, smaller, larger, pivot);
  smallEmpty = true;
  largeEmpty = true;
}
