#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"
#include "print_bst.h"

using namespace std;

int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('a',1));
    bt.insert(std::make_pair('b',2));
    bt.insert(std::make_pair('c',3));

    cout << "Binary Search Tree contents:" << endl;
    for(AVLTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(bt.find('b') != bt.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    bt.remove('b');
    cout << "Erasing a" << endl;
    bt.remove('a');
    cout << "Finding c" << endl;
    if(bt.find('c') != bt.end())
        cout << "Found c" << endl;
    cout << "Erasing c" << endl;
    bt.remove('c');
    for(AVLTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    
    // AVL Tree Tests
    AVLTree<char,int> at;
    at.insert(std::make_pair('a',1));
    cout << "a done" << endl;
    at.insert(std::make_pair('b',2));
    cout << "b done" << endl;
    at.insert(std::make_pair('c',3));
    cout << "c done" << endl;
    at.insert(std::make_pair('d',4));
    cout << "d done" << endl;

    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(at.find('b') != at.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    at.remove('b');

    return 0;
}
