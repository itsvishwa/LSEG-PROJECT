#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>

using namespace std;

bool isValidInstrument(const string& instrument);

bool isValidSide(int side);

bool isValidPrice(double price);

bool isValidQuantity(int qty);

#endif