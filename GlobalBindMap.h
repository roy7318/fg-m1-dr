#pragma once
#include <map>
#include <string>
using namespace std;

// Declare a global map for variable paths in the simulator.
extern map<string,double> pathMap;
extern map<string,string> vartopath;
extern bool timer;
extern bool mainTimer;