#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <Windows.h>

#include "headers/writefile.h"



using namespace std;

fstream fout("reportcard.csv", ios::out | ios::app);

string getCurrentDateTime() {
    // Get the current time
    auto now = chrono::system_clock::now();

    // Convert the current time to a time_t object
    time_t time = chrono::system_clock::to_time_t(now);

    // Convert the time_t object to a tm struct
    tm* tm = localtime(&time);

    // Format the date and time
    ostringstream oss;
    oss << put_time(tm, "%Y%m%d-%H%M%S");

    // Get the milliseconds part of the current time
    auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;

    // Append the milliseconds to the formatted date and time
    oss << "." << setfill('0') << setw(3) << ms.count();

    // Return the formatted date and time as a string
    return oss.str();
}

string getCurrentDateTime_win()
{
    SYSTEMTIME st;
    GetLocalTime(&st);

    char dateTime[20];
    sprintf_s(dateTime, "%04d%02d%02d-%02d%02d%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    return string(dateTime);
}

void writeLine(string orderId, string &clientId, int &instrument, int &side, int status, int &qty, double &price)
{
    static const vector<string> state = { "New", "Rejected", "Fill", "Pfill" };
    static const vector<string> instruments = { "Rose", "Lavender", "Lotus", "Tulip", "Orchid" };
  
    // Insert the data to file
    fout << orderId << ","
            << clientId << ","
            << instruments[instrument] << ","
            << side << ","
            << state[status] << ","
            << qty << ","
            << price << ","
            << getCurrentDateTime()
            << "\n";
    
}

void writeInvalidLine(string orderId, string &clientId, string &instrument, int &side, int &qty, double &price, vector<string>& invalidReasons)
{
    static string state = "Rejected";

    string reasons;
    for (const string& reason : invalidReasons) {
        reasons += reason + " | ";
    }
    reasons = reasons.substr(0, reasons.size() - 3);
    
    // Insert the data to file
    fout << orderId << ","
            << clientId << ","
            << instrument << ","
            << side << ","
            << state << ","
            << qty << ","
            << price << ","
            << reasons << ","
            << getCurrentDateTime()
            << "\n";
    
}