#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include "headers/validation.h"

using namespace std;

bool isValidInstrument(const string& instrument) {
    static const vector<string> validInstruments = { "Rose", "Lavender", "Lotus", "Tulip", "Orchid" };
    return find(validInstruments.begin(), validInstruments.end(), instrument) != validInstruments.end();
}

bool isValidSide(int side) {
    return side == 1 || side == 2;
}

bool isValidPrice(double price) {
    return price > 0.0;
}

bool isValidQuantity(int qty) {
    return qty >= 10 && qty <= 1000 && qty % 10 == 0;
}