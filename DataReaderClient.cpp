#pragma once
#include <netinet/in.h>
#include "Command.h"
#include "Thread.h"
#include "GlobalBindMap.h"
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <thread>
#include <pthread.h>

// Connects the client to thte server.
class DataReaderClient {
public:
    /**
     * @param args which represent the paremters which we recive from the command line.
     * @return  return the data which we recived from the client.
     */
    static void * ReadDataFromClient(void * args) {
        int port = ((params *) args)->port;
        string ip = ((params *) args)->ip;
        int sockfd, portno, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        char buffer[1024];
        portno = port;
        /* Create a socket point */
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("ERROR opening socket");
            exit(1);
        }
        server = gethostbyname(ip.c_str());
        if (server == NULL) {
            fprintf(stderr, "ERROR, no such host\n");
            exit(0);
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
        serv_addr.sin_port = htons(portno);
        // The timer is a global variable used to wait for the server to open.
        while(!timer) {
           // Wait for the timer to start.
        }
        cout << "The simulator has been opened. try to connect." << endl;
        /* Now connect to the server */
        if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            perror("ERROR connecting");
            exit(1);
        }
       cout << "Connected to the opened server." << endl;
        mainTimer = true;
        while (true) {
            //printf ("Please enter the message: ");
            bzero(buffer, 1024);
            //fgets(buffer, 1023, stdin);
            /* Send message to the server */
            // Declaring iterator to a vector
            map<string, double>::iterator itereli;
            // Displaying vector elements using begin() and end()
            for (itereli = pathMap.begin(); itereli != pathMap.end(); itereli++) {
                string setstr = "set ";
                string strpath = itereli->first;
                double value = itereli->second;
                std::ostringstream strs;
                strs << value;
                string valuedoublestr = strs.str();
                string strend = "\r\n";
                string allshit = setstr + strpath + " " + valuedoublestr + strend;
                strcpy(buffer,allshit.c_str());
                n = write(sockfd, buffer, strlen(buffer));
                if (n < 0) {
                    perror("ERROR writing to socket");
                    exit(1);
                }
                /* Now read server response */
                bzero(buffer, 1024);
                n = read(sockfd, buffer, 1023);
                if (n < 0) {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                printf("%s\n", buffer);
            }
        }

    }
};

