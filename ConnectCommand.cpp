#pragma once
#include "Command.h"
#include "Thread.h"
#include <netdb.h>
#include <thread>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "DataReaderClient.cpp"
#include "ShuntingYardParser.cpp"

class  ConnectCommand: public Command {
private:
    DataReaderClient client;
    ShuntingYardParser* shuntingYardParser;
public:

    void doCommand(vector<string> parameters) {
        string ip = parameters[1];
        int port = (int) shuntingYardParser->evaluate(parameters[2])->calculate();
        params *p = new params();
        p->port = port;
        p->ip = ip;
        pthread_t pthread;
        pthread_create(&pthread, nullptr, client.ReadDataFromClient, p);
       // pthread_join(pthread, nullptr);
        pthread_detach(pthread);

    }

    // Get the type of command.
    string getType() {
        return "ConnectCommand";
    }
    // Unused methods.
    // Get the map of variables from each command.
    virtual map<string,double> getSymbolTable() {
        map<string,double> empty;
        return empty;
    }
    // Set the map of variables for each command.
    virtual void setSymbolTable(map<string, double> m) {
        return;
    }
};








