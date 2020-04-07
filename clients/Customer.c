//
// Created by Aimen Fahmi on 2020-03-29.
//

#include "Customer.h"

#include "../util/client_utilities/ClientUtilities.h"
#include "../util/random_utilities/RandomUtilities.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>

#define INTERACTIVE_MODE 0
#define AUTOMATIC_MODE 1

// For an interactive interaction with the server
void interactiveMode(int client_socket) {
    char command[100];
    char item_name[100];
    char amount[100];

    while (1) {
        printf("Enter command (buy / write / exit): ");
        getLine("", command, sizeof(command));
        if (strcmp(command, "exit") == 0) {
            sendRequest(client_socket, "exit", sizeof("exit"));
            break;
        }
        printf("Enter item name: ");
        getLine("", item_name, sizeof(item_name));
        printf("Enter amount: ");
        getLine("", amount, sizeof(amount));

        if (strcmp(command, "buy") == 0) {
            requestServerToBuyItem(item_name, amount, sizeof(item_name), sizeof(amount), client_socket);
        } else if (strcmp(command, "write") == 0) {
            requestServerToWriteItem(item_name, amount, sizeof(item_name), sizeof(amount), client_socket);
        } else {
            printf("You typed something wrong ! try again...\n");
        }
    }
}

// For a scripted interaction with the server
void automaticMode(int client_socket, int customer_id) {
    char amount[12];
    sprintf(amount, "%d", customer_id);

    requestServerToWriteItem("customer", amount, sizeof("customer"), sizeof(amount), client_socket);

    requestServerToBuyItem("customer", amount, sizeof("customer"), sizeof(amount), client_socket);

    sendRequest(client_socket, "exit", sizeof("exit"));
}

// Sends a message to the server that looks like "writeItemToStore:key:cheese cake:amount:89"
int requestServerToWriteItem(const char *item_name, const char *amount, int item_name_size, int amount_size, int client_socket) {
    char message_to_send[sizeof("writeItemToStore:key:") + item_name_size + sizeof(":amount:") + amount_size];
    memset(message_to_send, 0, strlen(message_to_send));
    strcat(message_to_send, "writeItemToStore:key:");
    strcat(message_to_send, item_name);
    strcat(message_to_send, ":amount:");
    strcat(message_to_send, amount);

    if (sendRequest(client_socket, message_to_send, sizeof(message_to_send)) < 0) {
        printf("[-] Client was unable to send message: '%s'", message_to_send);
        exit(-1);
    }

    char acknowledgment[10];
    recv(client_socket, acknowledgment, sizeof(acknowledgment), 0);

    if (strcmp(acknowledgment, ITEM_WRITING_SUCCESS) == 0) {
        printf("[+] Customer successfully wrote item '%s' with count '%s' to the store\n", item_name, amount);
    } else if (strcmp(acknowledgment, ITEM_WRITING_FAILURE) == 0) {
        printf("[-] Customer was unable to write item '%s' with count '%s' to the store\n", item_name, amount);
    }

    return 0;
}

// Sends a message to the server that looks like "requestToBuyItem:key:cheese cake:amount:89"
int requestServerToBuyItem(const char *item_name, const char *amount, int item_name_size, int amount_size, int client_socket) {
    char message_to_send[sizeof("requestToBuyItem:key:") + item_name_size + sizeof(":amount:") + amount_size];
    memset(message_to_send, 0, strlen(message_to_send));
    strcat(message_to_send, "requestToBuyItem:key:");
    strcat(message_to_send, item_name);
    strcat(message_to_send, ":amount:");
    strcat(message_to_send, amount);

    if (sendRequest(client_socket, message_to_send, sizeof(message_to_send)) < 0) {
        printf("[-] Client was unable to send message: '%s'", message_to_send);
        exit(-1);
    }

    char acknowledgment[10];
    recv(client_socket, acknowledgment, sizeof(acknowledgment), 0);

    if (strcmp(acknowledgment, ITEM_BUYING_SUCCESS) == 0) {
        printf("[+] Customer was able to buy %s pieces of item '%s'\n", amount, item_name);
    } else if (strcmp(acknowledgment, ITEM_BUYING_FAILURE) == 0) {
        printf("[-] Customer was unable to buy %s pieces of item '%s' because the store doesn't have enough of them in stock\n", amount, item_name);
    } else if (strcmp(acknowledgment, ITEM_DOESNT_EXIST) == 0) {
        printf("[-] Customer was unable to buy %s pieces of item '%s' because the item doesn't exist in the store\n", amount, item_name);
    }

    return 0;
}

// Handles all requests coming from a specific client
// Most important part of the customer
void handleConnection(int mode, int customer_id) {
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
        automaticMode(socket, customer_id);
    } else if (mode == INTERACTIVE_MODE) {
        interactiveMode(socket);
    }

    close(socket);
    shutdown(socket,0);
    shutdown(socket,1);
    shutdown(socket,2);
}

int main(int argc, char *argv[]) {
    handleConnection(AUTOMATIC_MODE, atoi(argv[1]));
    return 0;
}
