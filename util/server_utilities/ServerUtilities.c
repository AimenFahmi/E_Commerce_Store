//
// Created by Aimen Fahmi on 2020-03-29.
//

#include "ServerUtilities.h"
#include "../exception_handling/Failure.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<arpa/inet.h>

// Returns descriptor of the socket if successfully created.
// Else, returns -1;
int createServerSocket() {
    int iSetOption = 1;
    int newSocket = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(newSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption,
               sizeof(iSetOption));

    if (newSocket == -1) {
        printf("[-] Socket creation failed\n");
        exit(-1);
    }
    return newSocket;
}

int bindCreatedSocket(int socket, unsigned int client_port) {
    struct sockaddr_in server_address = {0};

    server_address.sin_port = htons(client_port);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;

    int binding_status = bind(socket, (struct sockaddr *) &server_address, sizeof(server_address));

    if (binding_status < 0) {
        printf("[-] Binding to client port %d failed\n", client_port);
        exit(-1);
    }

    return binding_status;
}


void sendAcknowledgment(int client_socket, char *acknowledgment, int acknowledgment_size) {
    send(client_socket, acknowledgment, acknowledgment_size, 0);
}

