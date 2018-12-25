#pragma once
#include <netinet/in.h>
#include "Command.h"
#include "Thread.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <thread>
#include <pthread.h>
#include "GlobalBindMap.h"
#include "BufferParser.cpp"
#include "SleepCommand.cpp"

// Class opens the connection to the FlightGear simultaor.
class DataReaderServer {

public:
    // Open the server and read the data from it into a buffer.
    // Code taken from the Moodle example.
    static void * ReadDataFromServer(void * args) {
        //here we are goinmg to iterate all the patmap
        int port = ((params *) args)->port;
        int speed = ((params *) args)->speed;
        int sockfd, newsockfd, portno, clilen;
        char buffer[1024];
        struct sockaddr_in serv_addr, cli_addr;
        int n;
        int option = 1;
        /* First call to socket() function */
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        if (sockfd < 0) {
            perror("ERROR opening socket");
            exit(1);
        }
        /* Initialize socket structure */
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);
        /* Now bind the host address using bind() call.*/
        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            perror("ERROR on binding");
            exit(1);
        }
        /* Now start listening for the clients, here process will */
        cout << "Wait for the simulator to open." << endl;
        listen(sockfd, speed);
        clilen = sizeof(cli_addr);
        /* Accept actual connection from the client */
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
        timer = true;
        cout<<"Connected to the server." << endl;
        while (true) {
            if (newsockfd < 0) {
                perror("ERROR on accept");
                exit(1);
            }
            /* If connection is established then start communicating */
            bzero(buffer, 1024);
            n = read(newsockfd, buffer, 1023);
            if (n < 0) {
                perror("ERROR reading from socket");
                exit(1);
            }
            BufferParser* bufferParser1;
            bufferParser1->updatBindMap(buffer);
            /* Write a response to the client */
            if (n < 0) {
                perror("ERROR writing to socket");
                exit(1);
            };
            std::this_thread::sleep_for(0.2s);
        }
    }
};
