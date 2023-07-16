#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>

#include "headers/processOrder.h"
#include "headers/validation.h"
#include "headers/writefile.h"

using namespace std;
using namespace std::chrono;

static const vector<string> vec = { "Rose", "Lavender", "Lotus", "Tulip", "Orchid" };

int getInstrument(string& instrument){
    
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == instrument) {
            return static_cast<int>(i);  // Convert size_t to int for index
        }
    }
    return -1;
}



int main() {
    Order *order;
    ifstream file("order.csv");
    if (!file) {
        cout << "Failed to open the CSV file." << endl;
        return 1;
    }

    auto start = high_resolution_clock::now();

    string line;
    int orderId = 1; // Starting order ID
    int ValidCount = 0;
    int InvalidCount = 0;
    while (getline(file, line)) {
        istringstream iss(line);
        string clientId, instrument;
        int side, qty;
        double price;

        if (getline(iss, clientId, ',') &&
            getline(iss, instrument, ',') &&
            iss >> side &&
            iss.ignore() &&
            iss >> qty &&
            iss.ignore() &&
            iss >> price)
        {

            vector<string> invalidReasons;

            if (!isValidInstrument(instrument)) {
                invalidReasons.push_back("Invalid instrument");
            }
            if (!isValidSide(side)) {
                invalidReasons.push_back("Invalid side");
            }
            if (!isValidPrice(price)) {
                invalidReasons.push_back("Invalid price");
            }
            if (!isValidQuantity(qty)) {
                invalidReasons.push_back("Invalid quantity");
            }

            if (invalidReasons.empty()) {
                // Create an Order object
                order = new Order(to_string(orderId++), clientId, getInstrument(instrument), side, price, qty);

                // Print the order details
                // cout << "Created Order: " << order->orderId << ", " << order->clientId << ", " << vec[order->instrument]
                //           << ", " << order->side << ", " << order->price << ", " << order->qty << endl;
                
                processOrder(order);

                ValidCount++;
            }
            else
            {
                writeInvalidLine(to_string(orderId++), clientId, instrument, side, qty, price, invalidReasons);
                InvalidCount++;
            }
            

        }
    }

    file.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Valid orders: "<<ValidCount<<"\nInvalid orders: "<<InvalidCount<< endl;
    cout << "Total execution time: " << duration.count() << " milliseconds" << endl;

    return 0;
}
