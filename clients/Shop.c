//
// Created by Aimen Fahmi on 2020-03-29.
//

#include "Shop.h"
#include "../util/client_utilities/ClientUtilities.h"
#include "../util/random_utilities/RandomUtilities.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

#define INTERACTIVE_MODE 0
#define AUTOMATIC_MODE 1


// For an interactive interaction with the server
void interactiveMode(int client_socket) {
    char command[100];
    char item_name[100];
    char amount[100];

    while (1) {
        printf("Enter command (increase count / exit): ");
        getLine("", command, sizeof(command));
        if (strcmp(command, "exit") == 0) {
            sendRequest(client_socket, "exit", sizeof("exit"));
            break;
        }

        printf("Enter item name: ");
        getLine("", item_name, sizeof(item_name));
        printf("Enter amount: ");
        getLine("", amount, sizeof(amount));

        requestServerToIncreaseCountOfItem(item_name, amount, sizeof(item_name), sizeof(amount), client_socket);
    }
}

// For a scripted interaction with the server
void automaticMode(int client_socket) {

    for (int i = 0; i < 100; ++i) {
        char amount[12];
        sprintf(amount, "%d", getpid());

        requestServerToIncreaseCountOfItem("customer", amount, sizeof("customer"), sizeof(amount), client_socket);
    }

    sendRequest(client_socket, "exit", sizeof("exit"));
}

// Sends a message to the server that looks like "increaseCountOfItem:key:cheese:amount:56:"
int requestServerToIncreaseCountOfItem(const char *item_name, const char *amount, int item_name_size, int amount_size, int client_socket) {
    char message_to_send[sizeof("requestToBuyItem:key:") + item_name_size + sizeof(":amount:") + amount_size];
    memset(message_to_send, 0, strlen(message_to_send));
    strcat(message_to_send, "increaseCountOfItem:key:");
    strcat(message_to_send, item_name);
    strcat(message_to_send, ":amount:");
    strcat(message_to_send, amount);

    if (sendRequest(client_socket, message_to_send, sizeof(message_to_send)) < 0) {
        printf("[-] Client was unable to send message: '%s'", message_to_send);
        close(client_socket);
        exit(-1);
    }

    char acknowledgment[10];
    recv(client_socket, acknowledgment, sizeof(acknowledgment), 0);

    if (strcmp(acknowledgment, ITEM_COUNT_INCREASE_SUCCESS) == 0) {
        printf("[+] Shop successfully increased count of item '%s' by %s\n", item_name, amount);
    } else if (strcmp(acknowledgment, ITEM_COUNT_INCREASE_FAILURE) == 0) {
        printf("[-] Shop was unable to increase count of item '%s' by %s because the store contains more than 3 pieces of that item\n", item_name, amount);
    }

    return 0;
}

// Handles all requests coming from a specific client
// Most important part of the shop
void handleConnection(int mode) {
    int socket = createClientSocket();
    if (socket < 0) {
        printf("[-] Socket creation failed\n");
        exit(-1);
    }
    if (connectToServer(socket, PORT) < 0) {
        printf("[-] Connection to server on port %d failed\n", PORT);
        close(socket);
        exit(-1);
    }

    if (mode == AUTOMATIC_MODE) {
        automaticMode(socket);
    } else if (mode == INTERACTIVE_MODE) {
        interactiveMode(socket);
    }

    close(socket);
    shutdown(socket,0);
    shutdown(socket,1);
    shutdown(socket,2);
}

int main() {
    handleConnection(AUTOMATIC_MODE);
    return 0;
}
