#include <iostream>
#include <fstream>
#include <functional>
#include "llrec.h"
using namespace std;

/**
 * Reads integers (separated by whitespace) from a file
 * into a linked list.
 *
 * @param[in] filename
 *  The name of the file containing the data to read
 * @return
 *  Pointer to the linked list (or NULL if empty or the
 *  file is invalid)
 */
Node* readList(const char* filename);

/**
 * Prints the integers in a linked list pointed to
 * by head.
 */
void print(Node* head);

/**
 * Deallocates the linked list nodes
 */
void dealloc(Node* head);


Node* readList(const char* filename)
{
    Node* h = NULL;
    ifstream ifile(filename);
    int v;
    if( ! (ifile >> v) ) return h;
    h = new Node(v, NULL);
    Node *t = h;
    while ( ifile >> v ) {
        t->next = new Node(v, NULL);
        t = t->next;
    }
    return h;
}

void print(Node* head)
{
    while(head) {
        cout << head->val << " ";
        head = head->next;
    }
    cout << endl;
}

void dealloc(Node* head)
{
    Node* temp;
    while(head) {
        temp = head->next;
        delete head;
        head = temp;
    }
}

// -----------------------------------------------
//   Add any helper functions or
//   function object struct declarations
// -----------------------------------------------

struct Comp
{
  bool operator()(int a)
  {
    return a == 4;
  }
};



int main(int argc, char* argv[])
{
    if(argc < 2) {
        cout << "Please provide an input file" << endl;
        return 1;
    }

    // -----------------------------------------------
    // Feel free to update any code below this point
    // -----------------------------------------------
    Node* head = readList(argv[1]);
    cout << "Original list: ";
    print(head);

    // Test out your linked list code
    cout << "llpivot testing starting" << endl;
    Node* small = NULL;
    Node* large = NULL;
    llpivot(head, small, large, 7);

    cout << "small vals after pivot" << endl;
    print(small);
    cout << "large vals after pivot" << endl;
    print(large);
    cout << "llpivot testing over" << endl;


    cout << "llfilter testing beginning" << endl;
    Comp check4;

    small = llfilter(small, check4);
    cout << "small vals after filter" << endl;
    print(small);

    large = llfilter(large, check4);
    cout << "large vals after filter" << endl;
    print(large);

    //deallocating
    dealloc(large);
    dealloc(small);

    return 0;

}
