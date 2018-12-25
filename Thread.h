#pragma once
#include <string>
using namespace std;

// This struct is used to pass parameters to the threads.
struct params {
    // Server speed.
    int speed;
    // Port number.
    int port;
    // IP string.
    string ip;
};