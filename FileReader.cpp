#include <map>
#include <sstream>
#include "ConditionParser.cpp"
#include "OpenServerCommand.cpp"
#include "ConnectCommand.cpp"
#include "SleepCommand.cpp"
#include "DefineVarCommand.cpp"
#include "PrintCommand.cpp"
#include "LoopCommand.cpp"
#include "IfCommand.cpp"
#include "CommandExpression.cpp"

// Class in charge of reading from the file, creating and calling commands.
class FileReader {
    // The members of the class are the maps of commands and vars.
private:
    map <string, CommandExpression*> commandTable;
    map <string, double> symbolTable;
public:
    // Lexer function, converts a commands file to strings of each line.
    vector<string> lexer(string fileName) {
        // create a vector which represent the the lines of the file
        vector<string>vec;
        string STRING;
        ifstream infile;
        // opening the file
        infile.open (fileName);
        // To get you all the lines.
        while(!infile.eof()) {
            // Saves the line in STRING.
            getline(infile,STRING);
            vec.push_back(STRING);
        }
        infile.close();
        return vec;
    }

    // Partition the command lines to parameters.
    vector<string> removeSpaces(string &stringFunc){
        vector<string> newVec;
        vector<string>vec;
        istringstream iss(stringFunc);
        string s;
        // Remove the spaces.
        while (getline( iss, s, ' ' )) {
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

    //The parser creates the maps, one for commands and the other for variables.
    void parser (vector<string> vec) {
        commandTable.insert ( std::pair<string,CommandExpression*>("openDataServer", new CommandExpression));
        commandTable.insert ( std::pair<string,CommandExpression*>("connect",new CommandExpression));
        commandTable.insert ( std::pair<string,CommandExpression*>("var",new CommandExpression));
        commandTable.insert ( std::pair<string,CommandExpression*>("print",new CommandExpression));
        commandTable.insert ( std::pair<string,CommandExpression*>("sleep",new CommandExpression));
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
            // Remove the spaces from each line.
           // vector<string> line = removeSpaces(vec[i]);
            // Create a data server.
            if (line[0] == "openDataServer") {
                CommandExpression* command = commandTable.at(line[0]);
                command->setCommand(new OpenServerCommand);
                command->setVector(line);
                command->calculate();
                // Connect to a server.
            } else if (line[0] == "connect") {
                CommandExpression* command = commandTable.at(line[0]);
                command->setCommand(new ConnectCommand);
                command->setVector(line);
                command->calculate();
                // Wait for the connection to be established, pause the main thread.
                while(!mainTimer) {
                    // Do nothing.
                }
            } else if (line.size() >= 4 && line[3] == "bind" ) {
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
                vartopath.insert(pair<string, string>(line[1], line[4]));
                symbolTable.insert(pair<string, double>(line[1], 0));
                // Create a new var with numeric value.
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
                // Swap the value in the symbol table.
                auto it1 = symbolTable.find(line[0]);
                auto it2 = symbolTable.find(line[2]);
                if (it1 != symbolTable.end() && it2 != symbolTable.end()) {
                    it1->second = it2->second;
                }
                // If the value is bound to and existing path, change the value of the of the thing bound to that path.
                if (vartopath.count(line[0])==1) {
                    // Find the path  of bound to the variable.
                     map<string,string >:: iterator ita = vartopath.find(line[0]);
                     string path = ita->second;
                     // Find the path and it's variable.
                     map<string,double>:: iterator iter =pathMap.find(path);
                     // Change the value of the path.
                     iter->second=it2->second;
                }
                // Giving numeric value to existing var.
            } else if (symbolTable.count(line[0]) == 1 && is_number(line[2]) && line.size() <= 3 ) {
                auto it = symbolTable.find(line[0]);
                if (it != symbolTable.end()) {
                    it->second = stod(line[2]);
                }
                // Check if the changed var is bound to a path that also needes to be updated.
                if(vartopath.count(line[0])==1){
                    map<string,string >:: iterator ita = vartopath.find(line[0]);
                    string path = ita->second;
                    map<string,double>:: iterator iter = pathMap.find(path);
                    iter->second=stod(line[2]);
                }
                // If the var line contains and expression that needs to be calculated.
            } else if ((line[0] == "var" || symbolTable.count(line[0]) == 1) && checkExpression(line)) {
                CommandExpression* command = commandTable.at("var");
                command->setCommand(new DefineVarCommand);
                command->setVector(line);
                command->setMap(this->symbolTable);
                command->calculate();
                this->symbolTable = command->getMap();
                // Calling the print command.
            } else if (line[0] == "print") {
                vector<string> printLine;
                // If the parameter is a var, get it's value.
                if (symbolTable.count(line[1]) == 1) {
                    printLine.push_back(line[1]);
                    // If not, print as is.
                } else {
                    for(int j = 1; j < line.size(); j++) {
                        printLine.push_back(line[j]);
                    }
                }
                // Do the printing command.
                CommandExpression* command = commandTable.at(line[0]);
                command->setCommand(new PrintCommand);
                command->setVector(printLine);
                command->setMap(this->symbolTable);
                command->calculate();
                // The the line is a condition or a loop.
            } else if (line[0] == "while" || line[0] == "if") {
                // We take all the lines from the start of the condition/loop
                // And move them to the condition parser to deal with it
                // In a different location.
                h = i;
                conditionLines.push_back(vec[h]);
                // Count the opening bracket.
                // First line.
                if (vec[h].find('{')) {
                    lineCount ++;
                    h++;
                    // Second line.
                } else if (vec[h+1].find('{')) {
                    lineCount++;
                    h++;
                }
                // While we didn't close our condition/loop.
                // We count this by counting opening and closing brackets.
                // And going forward until the counter is 0.
                while(lineCount != 0) {
                    conditionLines.push_back(vec[h]);
                    if (vec[h].find('{') != std::string::npos) {
                        lineCount++;
                    }
                    if (vec[h].find('}') != std::string::npos) {
                        lineCount--;
                    }
                    h++;
                }
                // Create the condition parser and give him all the needed information.
                ConditionParser* conditionParser = new ConditionParser;
                conditionParser->setVariables(this->symbolTable);
                conditionParser->createCondition(conditionLines);
                // Get the updated information from the loop.
                this->symbolTable = conditionParser->getMap();
                // Continue from where to loop finished.
                i = h - 1;
                // If the line contains a sleep command..
            } else if (line[0] == "sleep") {
                CommandExpression* command = commandTable.at(line[0]);
                command->setCommand(new SleepCommand);
                command->setVector(line);
                command->calculate();
            }
        }
    }

    // Check if string is a number.
    bool is_number(const std::string& s) {
        try {
            std::stod(s);
        }
        catch(...) {
            return false;
        }
        return true;
    }

    // Returns the command table.
    map <string, CommandExpression*> getCommandTable() {
     return this->commandTable;
    }

    // Return the symbol table with the vars.
    map <string, double> getSymbolTable() {
        return  this->symbolTable;
    }

    // Check if the vector contains an expression.
    bool checkExpression(vector<string> vec) {
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