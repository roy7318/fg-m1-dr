#pragma once
#include "Command.h"
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "DataReaderServer.cpp"
#include <pthread.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <thread>
#include "Thread.h"
#include "ShuntingYardParser.cpp"

// The command that calls the server creation.
class OpenServerCommand : public Command {
private:
    // The server object.
    DataReaderServer server;
    // The object used to calculate the server parameters if they are expressions.
    ShuntingYardParser* shuntingYardParser;
public:
    // Calling this command creates a thread with the server.
    void doCommand(vector<string> parameters) {
        // Converting the parameters back to integers, using the expression parser.
        int port = (int) shuntingYardParser->evaluate(parameters[1])->calculate();
        int speed = (int) shuntingYardParser->evaluate(parameters[2])->calculate();
        // Placing the parameters into a struct.
        params *p = new params();
        p->port = port;
        p->speed = speed;
        // Creating the thread and calling the server creation  method.
        pthread_t pthread;
        pthread_create(&pthread, nullptr, server.ReadDataFromServer, p);
        //pthread_join(pthread, nullptr);
        pthread_detach(pthread);
    }

    // Get the type of command.
    string getType() {
            return "OpenServerCommand";
    }

    // Unused methods.
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