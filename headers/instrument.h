#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "processOrder.h"
#include "orderBook.h"

RBT* getBuybook(Order* order);
RBT* getSellbook(Order* order);

#endif