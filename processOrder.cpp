#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <chrono>
#include <ctime>
#include <iomanip>


#include "headers/processOrder.h"
#include "headers/orderBook.h"
#include "headers/writefile.h"
#include "headers/instrument.h"

using namespace std;

Order::Order(string orderId, string clientId, int instrument, int side, double price, int qty)
{
    //use initialize list
    this->orderId=orderId;
    this->clientId=clientId;
    this->instrument=instrument;
    this->side=side;
    this->price=price;
    this->qty=qty;
}

void processOrder(Order *order, int isResume){

    RBT *buyBook=getBuybook(order);
    RBT *sellBook=getSellbook(order);


    if (order->side==1)
    {
        Order *minSellOrder=sellBook->getMinOrder();
        if (minSellOrder==NULL)
        {
            if (isResume)
            {
                buyBook->insertNode(order);
            }
            else
            {
                buyBook->insertNode(order);
                writeLine(order->orderId, order->clientId, order->instrument, order->side, 0, order->qty, order->price);
            }
            
            
            return;
        }
        if (order->price<minSellOrder->price)
        {
            if (isResume)
            {
                buyBook->insertNode(order);

            }
            else
            {
                buyBook->insertNode(order);
                writeLine(order->orderId, order->clientId, order->instrument, order->side, 0, order->qty, order->price);
            }
            
            
        }
        else
        {
            if (minSellOrder->qty==order->qty)
            {
                writeLine(order->orderId, order->clientId, order->instrument, order->side, 2, order->qty, minSellOrder->price);
                
                writeLine(minSellOrder->orderId, minSellOrder->clientId, minSellOrder->instrument, minSellOrder->side, 2, minSellOrder->qty, minSellOrder->price);
                sellBook->deleteMinNode();
                return;
            }

            if (minSellOrder->qty>order->qty)
            {
                writeLine(order->orderId, order->clientId, order->instrument, order->side, 2, order->qty, minSellOrder->price);

                writeLine(minSellOrder->orderId, minSellOrder->clientId, minSellOrder->instrument, minSellOrder->side, 3, order->qty, minSellOrder->price);
                minSellOrder->qty-=order->qty;
                return;
            }

            if (minSellOrder->qty<order->qty)
            {
                writeLine(order->orderId, order->clientId, order->instrument, order->side, 3, minSellOrder->qty, minSellOrder->price);
                order->qty-=minSellOrder->qty;


                writeLine(minSellOrder->orderId, minSellOrder->clientId, minSellOrder->instrument, minSellOrder->side, 2, minSellOrder->qty, minSellOrder->price);
                sellBook->deleteMinNode();
                processOrder(order, 1);
                return;
            }
            
        }
        
        
    }
    else if (order->side==2)
    {
        
        Order *maxBuyOrder=buyBook->getMaxOrder();
        if (maxBuyOrder==NULL)
        {
            if (isResume)
            {
                sellBook->insertNode(order);
            }
            else
            {
                sellBook->insertNode(order);
                writeLine(order->orderId, order->clientId, order->instrument, order->side, 0, order->qty, order->price);
            }
            
            
            return;
        }
        if (order->price>maxBuyOrder->price)
        {
            if (isResume)
            {
                sellBook->insertNode(order);
            }
            else
            {
                sellBook->insertNode(order);
                writeLine(order->orderId, order->clientId, order->instrument, order->side, 0, order->qty, order->price);
            }
            
        }
        else
        {
            if (order->qty==maxBuyOrder->qty)
            {
                writeLine(order->orderId, order->clientId, order->instrument, order->side, 2, order->qty, maxBuyOrder->price);
                
                writeLine(maxBuyOrder->orderId, maxBuyOrder->clientId, maxBuyOrder->instrument, maxBuyOrder->side, 2, maxBuyOrder->qty, maxBuyOrder->price);
                buyBook->deleteMaxNode();
                return;
            }

            if (order->qty<maxBuyOrder->qty)
            {
                writeLine(order->orderId, order->clientId, order->instrument, order->side, 2, order->qty, maxBuyOrder->price);

                writeLine(maxBuyOrder->orderId, maxBuyOrder->clientId, maxBuyOrder->instrument, maxBuyOrder->side, 3, order->qty, maxBuyOrder->price);
                maxBuyOrder->qty-=order->qty;
                return;
            }

            if (order->qty>maxBuyOrder->qty)
            {
                writeLine(order->orderId, order->clientId, order->instrument, order->side, 3, maxBuyOrder->qty, maxBuyOrder->price);
                order->qty-=maxBuyOrder->qty;


                writeLine(maxBuyOrder->orderId, maxBuyOrder->clientId, maxBuyOrder->instrument, maxBuyOrder->side, 2, maxBuyOrder->qty, maxBuyOrder->price);
                buyBook->deleteMaxNode();
                processOrder(order, 1);
                return;
            }
        }
        
    }
}