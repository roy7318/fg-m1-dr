#pragma once
#include "Command.h"

// The class recreated the while command with the given stopping condition.
class  LoopCommand  {
private:
    // The map of variable.s
    map<string,double> m;
public:
// The doCommand function that starts the loop, inherited from the Command interface.
    void doCommand(vector<Command*> commands, vector<vector<string>> commandParams) {
            for (int i = 0; i < commands.size(); i++) {
                // If we print or define a variable we need to use our current map.
                if(commands[i]->getType() == "DefineVarCommand" || commands[i]->getType() == "PrintCommand") {
                    commands[i]->setSymbolTable(this->m);
                }
                commands[i]->doCommand(commandParams[i]);
                // If we redefined a variable wee need to update our map.
                if(commands[i]->getType() == "DefineVarCommand") {
                    this->m = commands[i]->getSymbolTable();
                }
            }
    }

    // Set the map.
    void setMap(map<string, double> m) {
        this->m = m;
    }

    // Return the map.
    map<string, double> getMap() {
    return this->m;
    }
};