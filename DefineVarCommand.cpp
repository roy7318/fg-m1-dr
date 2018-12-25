#pragma once
#include "Command.h"
#include "ShuntingYardParser.cpp"
#include <map>
#include "GlobalBindMap.h"
// Class defines variables that are expresions are updates them in the map.
class  DefineVarCommand: public Command {
private:
    map<string, double> symbolTable;
    ShuntingYardParser* yard;
public:
    // The doCommand function inherited from the Command interface.
    void doCommand(vector <string> parameters) {
        string str = connectVector(parameters);
        // vec[0] = the variable that will change, vec[1] = the value that will be calculated.
        vector<string> splittedExpression = splitExpression(str);
        string prep = getValues(splittedExpression[1]);
        double newVal = yard->evaluate(prep)->calculate();
        auto it = symbolTable.find(splittedExpression[0]);
        if (it != symbolTable.end()) {
            it->second = newVal;
        }
        // Change the value bound to the path if there is one.
        if(vartopath.count(splittedExpression[0])==1){
            map<string,string >:: iterator ita = vartopath.find(splittedExpression[0]);
            string path = ita->second;
            map<string,double>:: iterator iter =pathMap.find(path);
            iter->second=newVal;
        }
    }

    // Create a string from the vector.
    string connectVector(vector<string> parameters) {
        string result;
        for (int i =0; i < parameters.size(); i++) {
            result = result + parameters[i];
        }
        return result;
    }

    // Split the string at = to get both sides of the expression.
    vector<string> splitExpression(string splitMe) {
        std::vector<std::string> splittedString;
        int startIndex = 0;
        int endIndex = 0;
        while ((endIndex = splitMe.find("=", startIndex)) < splitMe.size()) {
            std::string val = splitMe.substr(startIndex, endIndex - startIndex);
            splittedString.push_back(val);
            startIndex = endIndex + 1;
        }
        if (startIndex < splitMe.size()) {
            std::string val = splitMe.substr(startIndex);
            splittedString.push_back(val);
        }
        return splittedString;
    }

    // Prepares the string for the shunting yard Algorithm.
    // Replaces the vars in the expression with their double values.
    string getValues(string str) {
        for (std::map<string, double>::iterator it = symbolTable.begin(); it != symbolTable.end(); ++it){
            if (str.find(it->first) != std::string::npos) {
                int val = str.find(it->first);
                string str1 = str.substr(0, val);
                string str2 = str.substr(val + it->first.length());
                std::ostringstream strs;
                strs << it->second;
                std::string str3 = strs.str();
                string res = str1 + str3 + str2;
                str = res;
            }
        }
        return str;
    }

    // Get the map of variables from each command.
    virtual map<string,double> getSymbolTable() {
        return this->symbolTable;
    }
    // Set the map of variables for each command.
    virtual void setSymbolTable(map<string, double> m) {
        this->symbolTable = m;
    }

    // Get the type of command.
    string getType() {
        return "DefineVarCommand";
    }
};