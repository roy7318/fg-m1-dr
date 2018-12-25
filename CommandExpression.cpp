#pragma once
#include "Expression.h"
#include "Command.h"

// The commands expression class used to call the calculate function on expressions.
class CommandExpression: public Expression {
private:
    // The command.
    Command* c;
    // The parameters of the command.
    vector<string> vec;
    // The map of variables and values.
    map<string, double> m;
public:
    // Set the command.
    void setCommand(Command* com) {
        this->c = com;
    }
    // Set the parameters.
    void setVector(vector<string> vec) {
        this->vec = vec;
    }
    // Set the variables.
    void setMap(map<string, double> m) {
        this->m = m;
    }
    // Calculate the command.
    double calculate() {
        c->setSymbolTable(this->m);
        c->doCommand(this->vec);
        this->m = c->getSymbolTable();
        return 0;
    }
    // Return the command.
    Command* getCommand(){
        return this->c;
    }
    // Return the variables.
    map<string, double> getMap() {
        return this->m;
    }
};