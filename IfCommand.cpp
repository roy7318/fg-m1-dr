#pragma once
#include "Command.h"

// The class recreated the while command with the given stopping condition.
class  IfCommand  {
private:
    // The map of variables.
    map<string,double> m;
public:
// The doCommand function that starts the loop, inherited from the Command interface.
    void doCommand(vector<Command*> commands, vector<vector<string>> commandParams) {
        for (int i = 0; i < commands.size(); i++) {
            // If the commands define a variable of print, we need to set our updated variables.
            if(commands[i]->getType() == "DefineVarCommand" || commands[i]->getType() == "PrintCommand") {
                commands[i]->setSymbolTable(this->m);
            }
            commands[i]->doCommand(commandParams[i]);
            // If we defined a var we need to update our variables.
            if(commands[i]->getType() == "DefineVarCommand") {
                this->m = commands[i]->getSymbolTable();
            }
        }
    }

    // Sets the map of variables.
    void setMap(map<string, double> m) {
        this->m = m;
    }
    // Gets the map of variables.
    map<string, double> getMap() {
        return this->m;
    }

};