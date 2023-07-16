#include "headers/processOrder.h"
#include "headers/orderBook.h"
#include <iostream>

using namespace std;


// orders saved as linked list as connected to the node in RBT

Node * RBT::createNode(Order *order){
    // 0-black, 1-red
    Node *node = new Node;
    node->color = 1;
    node->price = order->price;
    node->left = nullptr;
    node->right = nullptr;

    OrderNode *orderNode = new OrderNode;
    orderNode->order = order;
    orderNode->next = nullptr;
    node->head = orderNode;
    node->end = orderNode;
    return node;
}


void RBT::leftRotate(Node *node){

    Node * rightChild = node->right;
    Node * leftGrandChild = rightChild->left;

    if(node == root){
        root = rightChild;
        rightChild->left = node;
        node->right = leftGrandChild;

        //updaing parents
        rightChild->parent = nullptr;
        node->parent = rightChild;
        if(leftGrandChild != nullptr){
            leftGrandChild->parent = node;
        }
    }
    else{
        Node * parent = node->parent;
        bool isLeft = parent->left == node ? true : false;
        if(isLeft){
            parent->left = rightChild;
        }else{
            parent->right = rightChild;
        }
        rightChild->left = node;
        node->right = leftGrandChild;

        //updating parents
        rightChild->parent = parent;
        node->parent = rightChild;
        if(leftGrandChild != nullptr){
            leftGrandChild->parent = node;
        }
    }
}


void RBT::rightRotate(Node *node){

    Node * leftChild = node->left;
    Node * rightGrandChild = leftChild->right;

    if(node == root){
        root = leftChild;
        leftChild->right = node;
        node->left = rightGrandChild;

        // updating parents
        leftChild->parent = nullptr;
        node->parent = leftChild;
        if(rightGrandChild != nullptr){
            rightGrandChild->parent = node;
        }
    }else{
        Node * parent = node->parent;
        bool isLeft = parent->left == node ? true : false;
        if(isLeft){
            parent->left = leftChild;
        }else{
            parent->right = leftChild;
        }
        leftChild->right = node;
        node->left = rightGrandChild;

        // updating parents
        leftChild->parent = parent;
        node->parent = leftChild;
        if(rightGrandChild != nullptr){
            rightGrandChild->parent = node;
        }
    }
}


void RBT::insertNode(Order *order){
    // If RBT is empty
    if(root == nullptr){
        Node *node = createNode(order);
        root = node;
        node->parent = nullptr;
        node->color = 0; // set color as black

        //update Min & Max nodes values
        currMaxNode = node;
        currMinNode = node;
    }
    else{
        int price = order->price;
        Node *currNode = root;

        while (true){
            // if inserted order price less than current node's price
            if(price < currNode->price){
                if(currNode->left == nullptr){
                    currNode->left = createNode(order);
                    currNode->left->parent = currNode;

                    //update Min node
                    currMinNode = currMinNode->price > currNode->left->price ? currNode->left : currMinNode;

                    //fix height
                    insertViolationsFix(currNode->left);
                    break;
                }
                else{
                    currNode = currNode->left;
                }
            }
                // if inserted order price higher than current node's price
            else if(price > currNode->price){
                if(currNode->right == nullptr){
                    currNode->right = createNode(order);
                    currNode->right->parent = currNode;

                    //update Max node
                    currMaxNode = currMaxNode->price < currNode->right->price ? currNode->right : currMaxNode;

                    //fix height
                    insertViolationsFix(currNode->right);
                    break;
                }
                else{
                    currNode = currNode->right;
                }
            }
                // if inserted order price equal to current node's price
            else{
                // add a OrderNode to the end of the list
                OrderNode *orderNode = new OrderNode;
                orderNode->order = order;
                orderNode->next = nullptr;
                currNode->end->next = orderNode;
                currNode->end = orderNode;
                break;
            }
        }
    }
}


// to fix violations that encounter with insertions
void RBT::insertViolationsFix(Node *node){
    // 0-black, 1-red
    if(node == root){
        node->color  = 0;
    }
    else{
        Node *parent = node->parent;
        Node *grandParent = parent->parent;
        Node *uncle = nullptr;

        if(parent->color == 1){
            // find uncle node
            if(grandParent->left == parent){
                uncle = grandParent->right;
            }
            else{
                uncle = grandParent->left;
            }

            // checking uncle's color
            if(uncle == NULL || uncle->color == 0){
                // check whether a addtional rotaion is required or not
                bool isChildLeft, isParentLeft;
                isChildLeft = parent->left == node ? true : false;
                isParentLeft = grandParent->left == parent ? true : false;


                if(isParentLeft == !isChildLeft) // additional rotation required
                {
                    if(isParentLeft){
                        leftRotate(parent);
                    }else{
                        rightRotate(parent);
                    }
                    insertViolationsFix(parent);
                    return;
                }

                // perform second rotation
                if(isParentLeft){
                    rightRotate(grandParent);
                }else{
                    leftRotate(grandParent);
                }

                // changing colors
                grandParent->color = 1;
                parent->color = 0;

            }else{
                grandParent->color = 1;
                parent->color = 0;
                uncle->color = 0;
                insertViolationsFix(grandParent);
            }
        }
    }
}


// Largest node finding
Node * RBT::findLargest(Node *node){
    // this function should called if node have a left child. node can't has a right child in case of maximum node deletion event
    Node *curr = node->left;
    while(curr->right){
        curr = curr->right;
    }
    return curr;
}


// Smallest node finding
Node * RBT::findSmallest(Node *node){
    // this function should only called if a node has a right child. node can't has a left child in this case.
    Node *curr = node->right;
    while(curr->left){
        curr = curr->left;
    }
    return curr;
}


void RBT::deleteMinNode(){
    // Check weather the node has multiple orders in the linked list or not
    if(currMinNode->head == currMinNode->end) // If it has only one order
    {
        // minimum node can't has a left child
        if(currMinNode->right != nullptr) // has a right child
        {
            Node *nodeToDelete = findSmallest(currMinNode);

            // delete order node
            delete currMinNode->head->order;
            delete currMinNode->head;
            // replacing values
            currMinNode->head = nodeToDelete->head;
            currMinNode->end = nodeToDelete->end;
            currMinNode->price = nodeToDelete->price;

            // if we're trying to delete currMaxNode
            if(nodeToDelete == currMaxNode){
                currMaxNode = currMinNode;
            }

            deleteNode(nodeToDelete);
        }
        else // has no child
        {
            Node *temp = currMinNode;
            if(temp == root) // If node to be deleted is root without any child
            {
                currMinNode = nullptr;
                currMaxNode = nullptr;
            }

            else // update the minimum node
            {
                currMinNode = currMinNode->parent;
            }

            // delete order node
            delete temp->head->order;
            delete temp->head;

            temp->head = nullptr;
            temp->end = nullptr;

            deleteNode(temp);
        }
    }
    else // If it has multiple orders
    {
        //delete the first order
        OrderNode *temp = currMinNode->head;
        currMinNode->head = currMinNode->head->next;

        delete temp->order;
        delete temp;
    }
}


void RBT::deleteMaxNode(){
    // Check weather the node has multiple orders in the linked list or not
    if(currMaxNode->head == currMaxNode->end) // If it has only one order
    {
        // maximum node can't has a right child
        if(currMaxNode->left != nullptr) // has a left child
        {
            Node *nodeToDelete = findLargest(currMaxNode);

            // delete order node
            delete currMaxNode->head->order;
            delete currMaxNode->head;

            // replacing values
            currMaxNode->head = nodeToDelete->head;
            currMaxNode->end = nodeToDelete->end;
            currMaxNode->price = nodeToDelete->price;

            // if we're trying to delete currMinNode
            if(nodeToDelete == currMinNode){
                currMinNode = currMaxNode;
            }

            deleteNode(nodeToDelete);
        }
        else // has no child
        {
            Node *temp = currMaxNode;
            if(temp == root) // If node to be deleted is root without any child
            {
                currMinNode = nullptr;
                currMaxNode = nullptr;
            }

            else // update the minimum node
            {
                currMaxNode = currMaxNode->parent;
            }

            // delete order node
            delete temp->head->order;
            delete temp->head;

            temp->head = nullptr;
            temp->end = nullptr;

            deleteNode(temp);
        }
    }
    else // If it has multiple orders
    {
        //delete the first order
        OrderNode *temp = currMaxNode->head;
        currMaxNode->head = currMaxNode->head->next;

        delete temp->order;
        delete temp;
    }
}


void RBT::deleteNode(Node *node){
    // check the color
    if(node->color == 1) // if red
    {
        // check the node to be deleted has a child (not possible to have both children)
        bool HasLeftChild = node->left != nullptr ? true : false;
        bool HasRightChild = node->right != nullptr ? true : false;

        // check the node is left or right child of its parent
        bool isNodeLeftOfParent = node->parent->left == node ? true : false;

        if(isNodeLeftOfParent){
            if(HasLeftChild){
                node->parent->left = node->left;
                node->left->parent = node->parent;
            }
            else if(HasRightChild){
                node->parent->left = node->right;
                node->right->parent = node->parent;
            }
            else{
                node->parent->left = nullptr;
            }
        }else{
            if(HasLeftChild){
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
            else if(HasRightChild){
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
            else{
                node->parent->right = nullptr;
            }
        }
        delete node;
    }
    else // node to be deleted is black (require fixing)
    {
        if(node == root){
            if(node->left == nullptr && node->right == nullptr) // root with no child
            {
                delete node;
                root = nullptr;
            }
            else // root with one child
            {
                if(root->left == nullptr) // root has a right child
                {
                    root = node->right;
                    root->parent = nullptr;
                    delete node;
                }
                else // root has left chid
                {
                    root = node->left;
                    root->parent = nullptr;
                    delete node;
                }
            }
        }
        else{
            deletionFix(node, true);
        }
    }
}


// to fix violations that encounter with deletions
void RBT::deletionFix(Node *node, bool shouldDelete){
    // in recursion below statement can be true
    if(node == root){
        return;
    }
    Node *parent = node->parent;
    bool isNodeLeftOfParent = parent->left == node ? true : false;
    bool isSibilingLeftOfParent = !isNodeLeftOfParent;

    // find the sibiling node
    Node *sibiling = nullptr;
    if(isSibilingLeftOfParent){
        sibiling = parent->left;
    }else{
        sibiling = parent->right;
    }

    // Remind : sibling will not be null (because node is black)
    Node *sibilingLeftChild = sibiling->left;
    Node *sibilingRightChild = sibiling->right;
    int sibilingLeftChildColor = sibilingLeftChild == nullptr ? 0 : sibilingLeftChild->color;
    int sibilingRightChildColor = sibilingRightChild == nullptr ? 0 : sibilingRightChild->color;


    // Case 01 - sibiling red
    if(sibiling != nullptr && sibiling->color == 1)
    {
        parent->color = 1;
        sibiling->color = 0;
        if(isSibilingLeftOfParent){
            rightRotate(parent);
        }else{
            leftRotate(parent);
        }
        deletionFix(node, shouldDelete);
        return;
    }
        // Case 02 - sibiling black & both children black
    else if(sibiling->color == 0 && sibilingLeftChildColor == 0 && sibilingRightChildColor == 0)
    {
        sibiling->color = 1;

        // check the node to be deleted has a child (not possible to have both children)
        bool HasLeftChild = node->left != nullptr ? true : false;
        bool HasRightChild = node->right != nullptr ? true : false;

        if(shouldDelete){
            // remove node
            if(isNodeLeftOfParent){
                if(HasLeftChild){
                    parent->left = node->left;
                    node->left->parent = parent;
                }else if(HasRightChild){
                    parent->left = node->right;
                    node->right->parent = parent;
                }else{
                    parent->left = nullptr;
                }
            }else{
                if(HasLeftChild){
                    parent->right = node->left;
                    node->left->parent = parent;
                }else if(HasRightChild){
                    parent->right = node->right;
                    node->right->parent = parent;
                }else{
                    parent->right = nullptr;
                }
            }
            delete node;
        }


        // check parent color
        if(parent->color == 1){
            parent->color = 0;
        }// TODO :: check again
        else{
            deletionFix(parent, false);
            return;
        }
    }
        // Case 03 and 04 - sibiling black & one of the child of sibiling red
    else if(sibiling->color == 0)
    {
        if((isSibilingLeftOfParent && sibilingLeftChildColor == 0) || (!isSibilingLeftOfParent && sibilingRightChildColor == 0)){
            sibiling->color = 1;
            if(isSibilingLeftOfParent){
                sibilingRightChild->color = 0;
                leftRotate(sibiling);
            }else{
                sibilingLeftChild->color = 0;
                rightRotate(sibiling);
            }
            deletionFix(node, shouldDelete);
            return;
        }else{
            sibiling->color = parent->color;
            parent->color = 0;

            // check the node to be deleted has a child (not possible to have both children)
            bool HasLeftChild = node->left != nullptr ? true : false;
            bool HasRightChild = node->right != nullptr ? true : false;

            if(shouldDelete){
                // remove node
                if(isNodeLeftOfParent){
                    if(HasLeftChild){
                        parent->left = node->left;
                        node->left->parent = parent;
                    }else if(HasRightChild){
                        parent->left = node->right;
                        node->right->parent = parent;
                    }else{
                        parent->left = nullptr;
                    }
                }else{
                    if(HasLeftChild){
                        parent->right = node->left;
                        node->left->parent = parent;
                    }else if(HasRightChild){
                        parent->right = node->right;
                        node->right->parent = parent;
                    }else{
                        parent->right = nullptr;
                    }
                }
                delete node;
            }


            if(isSibilingLeftOfParent){
                sibilingLeftChild->color = 0;
                rightRotate(parent);
            }else{
                sibilingRightChild->color = 0;
                leftRotate(parent);
            }
        }
    }
}


int RBT::getMinPrice(){
    return currMinNode->price;
}


int RBT::getMaxPrice(){
    return currMaxNode->price;
}


// return the order with minimum price
Order * RBT::getMinOrder(){
    if(currMinNode == nullptr){
        return nullptr;
    }
    Order * temp = currMinNode->head->order;
    return temp;
}


// return the order with maximum price
Order * RBT::getMaxOrder(){
    if(currMaxNode == nullptr){
        return nullptr;
    }
    Order * temp = currMaxNode->head->order;
    return temp;
}


