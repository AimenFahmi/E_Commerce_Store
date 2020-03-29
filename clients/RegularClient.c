//
// Created by Aimen Fahmi on 2020-03-29.
//

#include "RegularClient.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<arpa/inet.h>

#define PORT 9005


// Returns descriptor of the socket if successfully created.
// Else, returns -1;
int createSocket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

// Returns 0 if successfully connected
// Returns -1 otherwise
int connectToServer(int socket, unsigned int server_port) {
    struct sockaddr_in server_address = {0};

    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);

    return connect(socket, (struct sockaddr *) &server_address, sizeof(server_address));
}


// Returns 0 if successfully sent
// Returns -1 otherwise
int sendRequest(int socket, char *request_message, int request_length) {
    return send(socket, request_message, request_length, 0);
}

// Returns 0 if successfully sent
// Returns -1 otherwise
int receiveResponse(int socket, char *to_store_response, int response_length) {
    return recv(socket, to_store_response, response_length, 0);
}

int main() {



    int socket = createSocket();

    if (socket == -1) {
        printf("[-] Socket creation failed\n");
        return -1;
    } else {
        printf("[+] Socket has been created\n");
    }

    int connection_status = connectToServer(socket, PORT);

    if (connection_status < 0) {
        printf("[-] Connection to server on port %d failed\n", PORT);
        return -1;
    } else {
        printf("[+] Client successfully connected to the server on port %d\n", PORT);
    }




    return 0;
}
