#include "headers/processOrder.h"
#include "headers/orderBook.h"
#include "headers/instrument.h"

using namespace std;

RBT RoseSellBook;
RBT RoseBuyBook;
RBT LavenderSellBook;
RBT LavenderBuyBook;
RBT LotusSellBook;
RBT LotusBuyBook;
RBT TulipSellBook;
RBT TulipBuyBook;
RBT OrchidSellBook;
RBT OrchidBuyBook;

RBT* getBuybook(Order* order){
    int i=order->instrument;
    switch (i)
    {
    case 0:
        return &RoseBuyBook;
        break;

    case 1:
        return &LavenderBuyBook;
        break;

    case 2:
        return &LotusBuyBook;
        break;

    case 3:
        return &TulipBuyBook;
        break;

    case 4:
        return &OrchidBuyBook;
        break;
    
    default:
        return nullptr;
        break;
    }
}

RBT* getSellbook(Order* order){
    int i=order->instrument;
    switch (i)
    {
    case 0:
        return &RoseSellBook;
        break;

    case 1:
        return &LavenderSellBook;
        break;

    case 2:
        return &LotusSellBook;
        break;

    case 3:
        return &TulipSellBook;
        break;

    case 4:
        return &OrchidSellBook;
        break;
    
    default:
        return nullptr;
        break;
    }
}