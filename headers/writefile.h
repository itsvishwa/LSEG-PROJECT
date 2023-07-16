#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <string>
#include <vector>

using namespace std;

string getCurrentDateTime();
void writeLine(string orderId, string &clientId, int &instrument, int &side, int status, int &qty, double &price);
void writeInvalidLine(string orderId, string &clientId, string &instrument, int &side, int &qty, double &price, vector<string>& invalidReasons);

#endif