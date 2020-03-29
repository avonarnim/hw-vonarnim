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

    if (temp == NULL)
      smaller->next = NULL;
    else
      smaller->next = temp;
  }
  else if (head->val > pivot)
  {
    temp = larger;
    larger = head;
    head = head->next;

    if (temp == NULL)
      larger->next = NULL;
    else
      larger->next = temp;
  }
  llpivot(head, smaller, larger, pivot);
}
