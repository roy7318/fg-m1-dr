#pragma once

#include <list>
#include "Command.h"
#include "OpenServerCommand.cpp"
#include "ConnectCommand.cpp"
#include "DefineVarCommand.cpp"
#include "PrintCommand.cpp"
#include "SleepCommand.cpp"
#include "LoopCommand.cpp"
#include "IfCommand.cpp"
#include "ShuntingYardParser.cpp"
#include "Bigger.cpp"
#include "Smaller.cpp"
#include "BiggerOrEqual.cpp"
#include "SmallerOrEqual.cpp"
#include "Equal.cpp"
#include "NotEqual.cpp"
#include <map>
#include <algorithm>

// Takes care of the if and while conditions.
class ConditionParser {
private:
    vector<Command*> commands;
    vector<vector<string>> commandVector;
    map<string, double> symbolTable;
    string condition;
    ShuntingYardParser* yard;
public:
    void createCondition(vector<string> vec) {
        this->condition = (vec[0]);
        vec.erase(vec.begin());
        vec.erase(vec.end());
        // Get each line from the command vector.
        // Used for counting the condition lines.
        int h = 0;
        for (int i = 0; i < vec.size(); i++) {
            // Stopping condition for line count in loops.
            int lineCount = 0;
            // Will hold all the loop command vectors.
            vector<string> conditionLines;
            //remove the spaces from variables.
            vector<string> line = removeSpaces(vec[i]);
            //remove the commas from the variables
            line =  removeCommas(vec[i]);
            //if threre is = there might be a problem so we are going to add nore spaces
            line = FixString(vec[i]);
            // Create a data server.
            if (line[0] == "openDataServer") {
                Command *server = new OpenServerCommand;
                commands.push_back(server);
                commandVector.push_back(line);
                // Connect to a server.
            } else if (line[0] == "connect") {
                Command *client = new ConnectCommand;
                commands.push_back(client);
                commandVector.push_back(line);
                // Wait for the connection to be established, pause the main thread.
                while(!mainTimer) {
                    // Do nothing.
                }
            } else if (line.size() >= 4 && line[3] == "bind") {
                if (line[4].find('\"') != std::string::npos) {
                    int val = line[4].find('\"');
                    line[4]= line[4].substr(val+1, line[4].length()-1);
                }
                if (line[4].find('\"') != std::string::npos) {
                    int val = line[4].find('\"');
                    line[4]= line[4].substr(0, val);
                }
                if(pathMap.count(line[4])==0){
                    pathMap.insert(pair<string, double>(line[4], 0));
                }
                symbolTable.insert(pair<string, double>(line[1], 0));
                Command *vars = new DefineVarCommand;
            } else if (line[0] == "var" && is_number(line[3])) {
                symbolTable.insert(pair<string, double>(line[1], stod(line[3])));
                // Giving new var value of existing var.
            } else if (line[0] == "var" && symbolTable.count(line[3]) == 1) {
                symbolTable.insert(pair<string, double>(line[1], 0));
                auto it1 = symbolTable.find(line[1]);
                auto it2 = symbolTable.find(line[3]);
                if (it1 != symbolTable.end() && it2 != symbolTable.end()) {
                    it1->second = it2->second;
                }
                // Giving value of existing var to existing var.
            } else if (symbolTable.count(line[0]) == 1 && symbolTable.count(line[2]) == 1 && line.size() == 3) {
                auto it1 = symbolTable.find(line[0]);
                auto it2 = symbolTable.find(line[2]);
                if (it1 != symbolTable.end() && it2 != symbolTable.end()) {
                    it1->second = it2->second;
                }
                // Update the path bound to the variable.
                if(vartopath.count(line[0])==1){
                    map<string,string >:: iterator ita = vartopath.find(line[0]);
                    string path = ita->second;
                    map<string,double>:: iterator iter = pathMap.find(path);
                    iter->second=it2->second;

                }
                // Giving numeric value to existing var.
            } else if (symbolTable.count(line[0]) == 1 && is_number(line[2]) && line.size() <= 3) {
                auto it = symbolTable.find(line[0]);
                if (it != symbolTable.end()) {
                    it->second = stod(line[2]);
                }
                if(vartopath.count(line[0])==1){
                    map<string,string >:: iterator ita = vartopath.find(line[0]);
                    string path = ita->second;
                    map<string,double>:: iterator iter = pathMap.find(path);
                    iter->second=stod(line[2]);
                }
                // If the number contains an expression.
            } else if ((line[0] == "var" || symbolTable.count(line[0]) == 1) && checkExpression(line)) {
                Command* command = new DefineVarCommand;
                commands.push_back(command);
                commandVector.push_back(line);
                // Calling the print command.
            } else if (line[0] == "print") {
                vector<string> printLine;
                // If the parameter is a var, get it's value.
                if (symbolTable.count(line[1]) == 1) {
                    printLine.push_back(line[1]);
                    // If not, print as is.
                } else {
                    for (int j = 1; j < line.size(); j++) {
                        printLine.push_back(line[j]);
                    }
                }
                // Do the printing command.
                Command *printer = new PrintCommand;
                commands.push_back(printer);
                commandVector.push_back(printLine);
                // Used for loops inside loops recursively.
            }  else if (line[0] == "while" || line[0] == "file") {
                conditionLines.push_back(vec[h]);
                if (vec[h].find('{')) {
                    lineCount++;
                    h++;
                } else if (vec[h + 1].find('{')) {
                    lineCount++;
                    h++;
                }
                while (lineCount != 0) {
                    conditionLines.push_back(vec[h]);
                    if (vec[h].find('{') != std::string::npos) {
                        lineCount++;
                    }
                    if (vec[h].find('}') != std::string::npos) {
                        lineCount--;
                    }
                    h++;
                }
                ConditionParser *conditionParser = new ConditionParser;
                conditionParser->createCondition(conditionLines);
                // The sleep commands, sleeps for assigned number of milliseconds.
            } else if (line[0] == "sleep") {
               Command* sleeper = new SleepCommand;
               commands.push_back(sleeper);
               commandVector.push_back(line);
            }
            h++;
        }
        // Check the type of loop and go into it,
        this->checkType();
    }

    // Check if a string is a number.
    bool is_number(const std::string &s) {
        try {
            std::stod(s);
        }
        catch (...) {
            return false;
        }
        return true;
    }

    // Remove the spaces from the string.
    vector<string> removeSpaces(string &stringFunc) {
        vector<string> newVec;
        vector<string> vec;
        istringstream iss(stringFunc);
        string s;
        // Remove the spaces.
        while (getline(iss, s, ' ')) {
            newVec.push_back(s);
        }
        // Check if to push the string to the new spaceless vector.
        for (auto &i : newVec) {
            if (i.empty()) {
                continue;
            } else {
                vec.push_back(i);
            }
        }
        return vec;
    }

    // Checks the type of loop and acts accordingly.
    void checkType() {
        if (this->condition.find("while") != std::string::npos) {
            fixCondition();
            LoopCommand *e = new LoopCommand;
            while (checkCondition()) {
                e->setMap(this->symbolTable);
                e->doCommand(commands, commandVector);
                this->symbolTable = e->getMap();
            }

        } else {
            IfCommand* m = new IfCommand;
            fixCondition();
            if (checkCondition()) {
                m->setMap(this->symbolTable);
                m->doCommand(commands, commandVector);
                this->symbolTable = m->getMap();
            }
        }
    }

    // Sets the map of variables and values.
    void setVariables(map<string, double> vars) {
        this->symbolTable = vars;
    }

    // Return the map.
    map<string,double> getMap() {
        return this->symbolTable;
    }

    // Checks the type of condition in the loop.
    bool checkCondition() {
        // Split cases for all types of conditions.
        if (condition.find(">=") != std::string::npos) {
            // Split the condition to two sides.
            vector<string> vec = splitExpression(">=");
            // Left side.
            vector<string>v = this->removeSpaces(vec[0]);
            // Right side.
            vector<string>v2 = this->removeSpaces(vec[1]);
            string s,s2;
            for (int i = 0; i <v.size() ; ++i) {
                s+=v[i];
            }
            for (int i = 0; i <v2.size() ; ++i) {
                s2+=v2[i];
            }
            // Transform the condition to a string (if there are vars they become their value) for Shunting yard.
            s = getValues(s);
            s2= getValues(s2);
            // Evaluate both sides of the condition with shunting yard.
            Expression *exp1 = yard->evaluate(s);
            Expression *exp2 = yard->evaluate(s2);
            // Check if the condition is correct.
            BiggerOrEqual *b = new BiggerOrEqual(exp1, exp2);
            // Return comparison result.
            return b->check();
            // Repeat for all types of conditions.
        } else if (condition.find("<=") != std::string::npos) {
            vector<string> vec = splitExpression("<=");
            vector<string>v = this->removeSpaces(vec[0]);
            vector<string>v2 = this->removeSpaces(vec[1]);
            string s,s2;
            for (int i = 0; i <v.size() ; ++i) {
                s+=v[i];
            }
            for (int i = 0; i <v2.size() ; ++i) {
                s2+=v2[i];
            }
            s = getValues(s);
            s2= getValues(s2);
            Expression *exp1 = yard->evaluate(s);
            Expression *exp2 = yard->evaluate(s2);
            SmallerOrEqual *b = new SmallerOrEqual(exp1, exp2);
            return b->check();
        } else if (condition.find('>') != std::string::npos) {
            vector<string> vec = splitExpression(">");
            vector<string>v = this->removeSpaces(vec[0]);
            vector<string>v2 = this->removeSpaces(vec[1]);
            string s,s2;
            for (int i = 0; i <v.size() ; ++i) {
                s+=v[i];
            }
            for (int i = 0; i <v2.size() ; ++i) {
                s2+=v2[i];
            }
            s = getValues(s);
            s2= getValues(s2);
            Expression *exp1 = yard->evaluate(s);
            Expression *exp2 = yard->evaluate(s2);
            Bigger *b = new Bigger(exp1, exp2);
            return b->check();
        } else if (condition.find('<') != std::string::npos) {
            vector<string> vec = splitExpression("<");
            vector<string>v = this->removeSpaces(vec[0]);
            vector<string>v2 = this->removeSpaces(vec[1]);
            string s,s2;
            for (int i = 0; i <v.size() ; ++i) {
                s+=v[i];
            }
            for (int i = 0; i <v2.size() ; ++i) {
                s2+=v2[i];
            }
            s = getValues(s);
            s2= getValues(s2);
            Expression *exp1 = yard->evaluate(s);
            Expression *exp2 = yard->evaluate(s2);
            Smaller *b = new Smaller(exp1, exp2);
            return b->check();
        } else if (condition.find("==") != std::string::npos) {
            vector<string> vec = splitExpression("==");
            vector<string>v = this->removeSpaces(vec[0]);
            vector<string>v2 = this->removeSpaces(vec[1]);
            string s,s2;
            for (int i = 0; i <v.size() ; ++i) {
                s+=v[i];
            }
            for (int i = 0; i <v2.size() ; ++i) {
                s2+=v2[i];
            }
            s = getValues(s);
            s2= getValues(s2);
            Expression *exp1 = yard->evaluate(s);
            Expression *exp2 = yard->evaluate(s2);
            Equal *b = new Equal(exp1, exp2);
            return b->check();
        } else if (condition.find("!=") != std::string::npos) {
            vector<string> vec = splitExpression("!=");
            vector<string>v = this->removeSpaces(vec[0]);
            vector<string>v2 = this->removeSpaces(vec[1]);
            string s,s2;
            for (int i = 0; i <v.size() ; ++i) {
                s+=v[i];
            }
            for (int i = 0; i <v2.size() ; ++i) {
                s2+=v2[i];
            }
            s = getValues(s);
            s2= getValues(s2);
            Expression *exp1 = yard->evaluate(s);
            Expression *exp2 = yard->evaluate(s2);
            NotEqual *b = new NotEqual(exp1, exp2);
            return b->check();
        }
    }

    // Takes the line of the condition and fixes it into a usable format.
    void fixCondition() {
        // Remove the condition word and change the condition string.
        if (condition.find("if") != std::string::npos) {
            int val = condition.find("if");
            this->condition = condition.substr(val + 3);
        }
        if (condition.find("while") != std::string::npos) {
            int val = condition.find("while");
            this->condition = condition.substr(val + 5);
        }
        // Also remove the useless '{' character.
        if (condition.find('{') != std::string::npos) {
            int val = condition.find('{');
            this->condition = condition.substr(0, val);
        }
    }

    // Splits the expression into both sides, based on the appropriate sign of the expression.
    vector<string> splitExpression(string sign) {
        std::vector<std::string> splittedString;
        int startIndex = 0;
        int endIndex = 0;
        while ((endIndex = condition.find(sign, startIndex)) < condition.size()) {
            std::string val = condition.substr(startIndex, endIndex - startIndex);
            splittedString.push_back(val);
            startIndex = endIndex + sign.size();

        }
        if (startIndex < condition.size()) {
            std::string val = condition.substr(startIndex);
            splittedString.push_back(val);
        }
        return splittedString;
    }

    // Replaces the string name of the var in the expression string with it's double value for Shunting Yard.
    string getValues(string str) {
        // Iterate the map to find the var.
        for (std::map<string, double>::iterator it = symbolTable.begin(); it != symbolTable.end(); ++it){
                if (str.find(it->first) != std::string::npos) {
                    // If the var was founds, split it to two sides.
                    // Before the var.
                    int val = str.find(it->first);
                    string str1 = str.substr(0, val);
                    // After the var.
                    string str2 = str.substr(val + it->first.length());
                    std::ostringstream strs;
                    strs << it->second;
                    // Convert the value of the var to a string.
                    std::string str3 = strs.str();
                    // Put them back together to create a new usable expression string.
                    string res = str1 + str3 + str2;
                    // Continue untill all vars are replaced by their values.
                    str = res;
                }
        }
                // Return the new usable string.
                return str;
    }

    // Check if the vector contains an expression.
    bool checkExpression(vector<string> vec) {
        // Check if the vector complains any complex expression that need to be calculated.
        // Does this by searching for a mathematical symbol in the parts of the vector.
        // Returns true if found.
        for (int p =  0; p < vec.size(); p++) {
            if (vec[p].find('+') != std::string::npos) {
                return true;
            }
            if (vec[p].find('-') != std::string::npos) {
                return true;
            }
            if (vec[p].find('*') != std::string::npos) {
                return true;
            }
            if (vec[p].find('/') != std::string::npos) {
                return true;
            }
        }
        // Else return false.
        return false;
    }
    //find the char c which the function gets.
    int FindChar(string str,char c){
        for (int i = 0; i < str.size(); ++i) {
            if(str[i]==c){
                return i;
            }
        }
        return -1;

    }
    //delete the spaces between the sentences
    vector<string> FixString(string str){
        if ((str.find("if") != std::string::npos)||(str.find("while") != std::string::npos) ) {
            vector<string> v =removeSpaces(str);
            return v;
        }
        int val = FindChar(str,'=');
        if(val==-1){
            return removeSpaces(str);
        }
        string str1 =str.substr(0,val)+" ";
        string str2 = " " + str.substr(val+1,str.size());
        string str3 = " = ";
        string str4 = str1 + str3+str2;
        vector<string> v =removeSpaces(str4);
        return v;
    }

    //remove the commas between our words.
    vector<string> removeCommas(string str) {
        for (int i = 0; i < str.length(); i++) {
            if(str[i] == ','){
                str[i] = ' ';
            }
        }
        return removeSpaces(str);
    }

};