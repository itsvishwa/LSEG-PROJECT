#ifndef PROCESSORDER_H
#define PROCESSORDER_H

#include <string>

using namespace std;

class Order
{
    
public:
    string orderId;
    string clientId;
    int instrument;
    int side;
    double price;
    int qty;

    Order(string orderId, string clientId, int instrument, int side, double price, int qty);

};


void processOrder(Order *order, int isResume=0);

#endif