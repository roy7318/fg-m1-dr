#pragma once
#include "Command.h"
#include <algorithm>

// The PrintCommand class handles the printing command.
class  PrintCommand: public Command {
private:
    // The variable map.
    map<string,double> symbolTable;
public:
    // Print the vector of parameters.
    void doCommand(vector<string> parameters ) {
        if (symbolTable.count(parameters[0]) == 1) {
            auto it1 = symbolTable.find(parameters[0]);
            cout << it1->second;
        }
        else {
            // Iterate vectors with range based loop.
            for (auto i : parameters) {
                // Erase the " characters.
                i.erase(std::remove(i.begin(), i.end(), '\"'), i.end());
                // Print the word.
                cout << i << " ";
            }
        }
        // Break the line.
        cout << endl;
    }

    // Get the type of command.
    string getType() {
        return "PrintCommand";
    }

    // Get the map of variables from each command.
    virtual map<string,double> getSymbolTable() {
        return this->symbolTable;
    }
    // Set the map of variables for each command.
    virtual void setSymbolTable(map<string,double> m) {
        this->symbolTable =  m;
    }
};
