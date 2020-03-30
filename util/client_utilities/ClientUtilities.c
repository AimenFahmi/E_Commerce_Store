//
// Created by Aimen Fahmi on 2020-03-29.
//

#include "ClientUtilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<arpa/inet.h>


// Returns descriptor of the socket if successfully created.
// Else, returns -1;
int createClientSocket() {
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

int talkToServer(unsigned int server_port, char *message_to_send, int message_length) {
    int socket = createClientSocket();
    int error_flag = 0;

    if (socket == -1) {
        printf("[-] Socket creation failed\n");
        error_flag = -1;
        goto close;
    } else {
        printf("[+] Socket has been created\n");
    }

    int connection_status = connectToServer(socket, server_port);

    if (connection_status < 0) {
        printf("[-] Connection to server on port %d failed\n", server_port);
        error_flag = -1;
        goto close;
    } else {
        printf("[+] Client successfully connected to the server on port %d\n", server_port);
    }

    int sending_status = sendRequest(socket, message_to_send, message_length);

    if (sending_status < 0) {
        printf("[-] Client was unable to send message: '%s'", message_to_send);
        error_flag = -1;
        goto close;
    }

    char buffer[10];
    recv(socket, buffer, sizeof(buffer), 0);

    if (buffer[0] == '1') {
        error_flag = -2;
    }

    close: close(socket);
    shutdown(socket,0);
    shutdown(socket,1);
    shutdown(socket,2);

    return error_flag;
}
