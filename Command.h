#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "vector"
#include "string"
#include "map"

using namespace std;
// The command interface, inherited by all methods that have a command to do.
class Command {
public:
    // Virtual method that calls the command.
    virtual void doCommand(vector<string> parameters )=0;
    // Virtual method that returns the type of the command.
    virtual string getType()=0;
    // Get the map of variables from each command.
    virtual map<string,double> getSymbolTable() = 0;
    // Set the map of variables for each command.
    virtual void setSymbolTable(map<string, double> m) = 0;
};

