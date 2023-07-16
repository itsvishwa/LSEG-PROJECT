#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "processOrder.h"

struct OrderNode{
    Order *order;
    OrderNode *next;
};


struct Node{
    int price;
    Node *left;
    Node *right;
    Node *parent;
    // 0-black, 1-red
    int color;
    // orders saved as a linked list
    OrderNode *head;
    OrderNode *end;
};


class RBT{
private:
    Node *root = nullptr;
    Node *currMaxNode = nullptr;
    Node *currMinNode = nullptr;

    Node * createNode(Order *order);
    void leftRotate(Node *node);
    void rightRotate(Node *node);
    void insertViolationsFix(Node *node);
    void deleteNode(Node *node);
    void deletionFix(Node *node, bool shouldDelete);
    Node * findLargest(Node *node);
    Node * findSmallest(Node *node);

public:
    void insertNode(Order *order);
    void deleteMinNode();
    void deleteMaxNode();
    int getMinPrice();
    int getMaxPrice();
    Order * getMinOrder();
    Order * getMaxOrder();

};

#endif