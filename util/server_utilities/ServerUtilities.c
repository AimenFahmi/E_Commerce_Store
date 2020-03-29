//
// Created by Aimen Fahmi on 2020-03-29.
//

#include "ServerUtilities.h"

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
    return socket(AF_INET, SOCK_STREAM, 0);
}

int bindCreatedSocket(int socket, unsigned int client_port) {
    struct sockaddr_in server_address = {0};

    server_address.sin_port = htons(client_port);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;

    return bind(socket, (struct sockaddr *) &server_address, sizeof(server_address));
}

int talkToClient(unsigned int client_port) {
    char message_to_receive[100] = {0};

    int socket = createServerSocket();

    if (socket == -1) {
        printf("[-] Socket creation failed\n");
        return -1;
    } else {
        printf("[+] Socket has been created\n");
    }

    int binding_status = bindCreatedSocket(socket, client_port);

    if (binding_status < 0) {
        printf("[-] Binding to client port %d failed\n", client_port);
        return -1;
    } else {
        printf("[+] Server successfully bound to the client on port %d\n", client_port);
    }

    listen(socket, 5);

    printf("[+] Waiting for connections ...\n");

    while (1) {
        memset(message_to_receive, '\0', sizeof(message_to_receive));


        int client_socket = accept(socket, NULL, NULL);

        if (client_socket < 0) {
            printf("[-] Could not accept client on port %d\n", client_port);
            return -1;
        }

        int message_reception_status = recv(client_socket, message_to_receive, sizeof(message_to_receive), 0);

        if (message_reception_status < 0) {
            printf("[-] Could not receive message from client on port %d\n", client_port);
            return -1;
        } else if (strcmp(message_to_receive, "exit") == 0) {
            close(client_socket);
            break;
        } else {
            printf("[+] Message from client on port %d: '%s'\n", client_port, message_to_receive);
        }

    }

    printf("[+] Server has become disconnected\n");
    return 0;
}

