#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
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
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
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
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
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
    void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* g);
    void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* p);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    // empty
    if (root_==nullptr) {
        AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        root_ = newNode;
        return;
    }
    // else tree not empty
    bool done = false;
    AVLNode<Key, Value>* current = root_;
    while(!done) { 
        if (current->getKey()==new_item.first) {
            current->setValue(new_item.second);
            // updating doesn't change balance, no need to fix tree
            return;
        }
         // figure out whether to move right or left
        // left
        if (new_item.first < current->getKey()) {
            // check if there's an empty spot there
            if (current->getLeft()==NULL) {
                // insert it here
                AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, current);
                current->setLeft(newNode);
                done = true;
            }
            else { // current has a left child
                current = current->getLeft();
            }
        }
        // right
        else {
            // check if there's an empty spot there
            if (current->getRight()==NULL) {
                // insert it here
                AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, current);
                current->setRight(newNode);
                done = true;
            }
            else { // current has a left child
                current = current->getRight();
            }
        }
    }
    // exited loop means newNode was created
    // use newNode parent pointer to take care of tree balance
    // newNode must have parent, or else it was the root
    if ((newNode->getParent()->getBalance()==-1) || (newNode->getParent()->getBalance()==1)) {
        newNode->getParent()->setBalance(0);
        return;
    }
     // b(p) = 0
    if (newNode->getParent()->getRight()==newNode) // we just added a right child
        newNode->getParent()->updateBalance(1); // right-left just increased by 1
    // else we just added a left child
    else
        newNode->getParent()->updateBalance(-1); // right-left just decreased by 1
    
    insertFix(newNode->getParent(), newNode);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n) {
    if (p==nullptr)
        return;
    AVLNode<Key, Value>* g = p->getParent();
    if (g==nullptr)
        // ?
    
    // if p is left child of g
    if (g->getLeft()==p) {
        g->updateBalance(-1); // added something to its left side -> right-left decreased
        if (g->getBalance()==0)
            return;
        if (g->getBalance()==-1)
            insertFix(g, p);
        else { // b(g) = -2
            if (p->getLeft()==n) { // zig-zig then rotateRight(g); b(p) = b(g) = 0
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else { // zig-zag
                rotateLeft(p);
                rotateRight(g);
                if (n->getBalance()==-1) {
                    p->setbalance(0);
                    g->setBalance(1);
                }
                else if (n->getBalance()==0) {
                    p->setBalance(0);
                    g->setBalance(0);
                }
                else if (n->getBalance()==1) {
                    p->setBalance(-1);
                    g->setBalance(0);
                }
                n->setBalance(0);
            }
        }

    }
    else { // p is right child of g
        g->updateBalance(1); // added something to its left side -> right-left decreased
        if (g->getBalance()==0)
            return;
        if (g->getBalance()==-1)
            insertFix(g, p);
        else { // b(g) = 2
            if (p->getRight()==n) { // zig-zig then rotateLeft(g); b(p) = b(g) = 0
                rotateLeftt(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else { // zig-zag
                rotateRight(p);
                rotateLeft(g);
                if (n->getBalance()==1) {
                    p->setbalance(0);
                    g->setBalance(-1);
                }
                else if (n->getBalance()==0) {
                    p->setBalance(0);
                    g->setBalance(0);
                }
                else if (n->getBalance()==-1) {
                    p->setBalance(1);
                    g->setBalance(0);
                }
                n->setBalance(0);
            }
        }
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* g) {
    AVLNode* y = g->getLeft();
    AVLNode* c = y->getRight(); // the whole c subtree
    y->setRight(g);
    g->setParent(y);
    g->setLeft(c);
    c->setParent(g);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* p) {
    AVLNode* y = g->getRight();
    AVLNode* b = y->getLeft(); // the whole b subtree
    y->setLeft(g);
    g->setParent(y);
    g->setRight(b);
    b->setParent(g);
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
