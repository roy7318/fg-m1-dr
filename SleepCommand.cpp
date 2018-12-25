#pragma once
#include <unistd.h>
#include "Command.h"

// The PrintCommand class handles the printing command.
class  SleepCommand: public Command {
private:
    unsigned int milliSecods;
public:
    // Print the vector of parameters.
    void doCommand(vector<string> parameters ) override {
        int x = stoi(parameters[1]);
        x = x * 1000;
        usleep(x);
    }

    // Get the type of command.
    string getType() {
        return "SleepCommand";
    }

    // Get the map of variables from each command.
    virtual map<string,double> getSymbolTable() {
        map<string,double> empty;
        return empty;
    }
    // Set the map of variables for each command.
    virtual void setSymbolTable(map<string,double> m) {
        return;
    }
};
