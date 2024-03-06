#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int countChildren(Node *root);

bool equalPaths(Node * root)
{
    int childrenCount = countChildren(root);
    if (childrenCount==-1)
        return false;
    return true;
    // // base case: root is leaf
    // if ((root->left==nullptr) && (root->right==nullptr)) {
    //     return true;
    // }
    // // only 1 child
    // if (root->left==nullptr) {
    //     return equalPaths(root->right);
    // }
    // if (root->right==nullptr) {
    //     return equalPaths(root->left);
    // }
    // // 2 children
    // // CHECK IF # PATHS UP TO ROOT ARE EQUAL
    // return (countChildren(root->left, 0) == countChildren(root->right, 0));
    // return ((equalPaths(root->left)) && (equalPaths(root->right)));

}

int countChildren(Node *root) {
    // is a leaf
    int rightC, leftC;
    if ((root->left==nullptr) && (root->right==nullptr))
        return 1;

    // one child. return that child count + 1. unless equal paths rule was broken in lower subtree
    else if (root->left==nullptr) {
        rightC = countChildren(root->right);
        if (rightC==-1)
            return -1;
        return 1 + rightC;
    }
    else if (root->right==nullptr) {
        leftC = countChildren(root->left);
        return 1 + leftC;
    }
    else {
        leftC = countChildren(root->left);
        rightC= countChildren(root->right);
        if (leftC != rightC)
            return -1;
        else
            return 1+ leftC;
    }
    
}

