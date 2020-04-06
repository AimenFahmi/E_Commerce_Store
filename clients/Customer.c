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

#define INTERACTIVE_MODE 0
#define AUTOMATIC_MODE 1

void interactiveMode(void) {
    char command[100];
    char item_name[100];
    char amount[100];

    while (1) {
        printf("Enter command (buy / write): ");
        getLine("", command, sizeof(command));
        if (strcmp(command, "exit") == 0) {
            talkToServer(PORT, "exit", sizeof("exit"));
            break;
        }
        printf("Enter item name: ");
        getLine("", item_name, sizeof(item_name));
        printf("Enter amount: ");
        getLine("", amount, sizeof(amount));

        if (strcmp(command, "buy") == 0) {
            requestServerToBuyItem(item_name, amount, PORT);
        } else if (strcmp(command, "write") == 0) {
            requestServerToWriteItemToStore(item_name, amount, PORT);
        } else {
            printf("You entered something wrong ! try again...\n");
        }
    }
}

void automaticMode(void) {
    for (int i = 0; i < 100; ++i) {
        msleep(i*10);
        char count[12];
        sprintf(count, "%d", i);
        requestServerToWriteItemToStore("aimen", count, PORT);
    }
    talkToServer(PORT, "exit", sizeof("exit"));
}

int writingRequest(const char *item_name, const char *amount, int client_socket) {
    char message_to_send[strlen("writeItemToStore:key:") + strlen(item_name) + strlen(":") + strlen("amount:") + strlen(amount)];
    memset(message_to_send, 0, strlen(message_to_send));
    strcat(message_to_send, "writeItemToStore:key:");
    strcat(message_to_send, item_name);
    strcat(message_to_send, ":");
    strcat(message_to_send, "amount:");
    strcat(message_to_send, amount);

    if (sendRequest(client_socket, message_to_send, sizeof(message_to_send)) < 0) {
        printf("[-] Client was unable to send message: '%s'", message_to_send);
        return -1;
    }

    char acknowledgment[10];
    recv(client_socket, acknowledgment, sizeof(acknowledgment), 0);

    if (strcmp(acknowledgment, ITEM_WRITING_SUCCESS) == 0) {
        printf("[+] Client successfully wrote item '%s' with count '%s' to the store\n", item_name, amount);
    } else if (strcmp(acknowledgment, ITEM_WRITING_FAILURE) == 0) {
        printf("[-] Client was unable to write item '%s' with count '%s' to the store\n", item_name, amount);
    }

    return 0;
}

int buyingRequest(const char *item_name, const char *amount, int client_socket) {
    char message_to_send[strlen("requestToBuyItem:key:") + strlen(item_name) + strlen(":") + strlen("amount:") + strlen(amount)];
    memset(message_to_send, 0, strlen(message_to_send));
    strcat(message_to_send, "requestToBuyItem:key:");
    strcat(message_to_send, item_name);
    strcat(message_to_send, ":");
    strcat(message_to_send, "amount:");
    strcat(message_to_send, amount);

    if (sendRequest(client_socket, message_to_send, sizeof(message_to_send)) < 0) {
        printf("[-] Client was unable to send message: '%s'", message_to_send);
        return -1;
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

void handleConnection(int mode) {
    int socket = createClientSocket();
    if (socket < 0) {
        printf("[-] Socket creation failed\n");
        goto close;
    }
    if (connectToServer(socket, PORT) < 0) {
        printf("[-] Connection to server on port %d failed\n", PORT);
        goto close;
    }

    if (mode == AUTOMATIC_MODE) {
        for (int i = 0; i < 100; ++i) {
            msleep(100);
            char amount[12];
            sprintf(amount, "%d", i);
            writingRequest("aimen", amount, socket);
            buyingRequest("aimen", amount, socket);
        }
        sendRequest(socket, "exit", sizeof("exit"));
    }

    close: close(socket);
    shutdown(socket,0);
    shutdown(socket,1);
    shutdown(socket,2);
}

// Message will look like: "writeItemToStore:key:cheese:amount:56"
int requestServerToWriteItemToStore(const char *item_name, const char *amount, int server_port) {
    char message_to_send[strlen("writeItemToStore:key:") + strlen(item_name) + strlen(":") + strlen("amount:") + strlen(amount)];
    memset(message_to_send, 0, strlen(message_to_send));
    strcat(message_to_send, "writeItemToStore:key:");
    strcat(message_to_send, item_name);
    strcat(message_to_send, ":");
    strcat(message_to_send, "amount:");
    strcat(message_to_send, amount);

    int talking_status = talkToServer(server_port, message_to_send, sizeof(message_to_send));

    if (talking_status == atoi(ITEM_WRITING_SUCCESS)) {
        printf("[+] Client successfully wrote item '%s' with count '%s' to the store\n", item_name, amount);
    } else if (talking_status == atoi(ITEM_WRITING_FAILURE)) {
        printf("[-] Client was unable to write item '%s' with count '%s' to the store\n", item_name, amount);
    } else if (talking_status == NETWORK_FAILURE) {
        printf("[-] Client was unable to talk to server on port %d and therefore, unable to write item '%s' to the store", server_port, item_name);
        return -1;
    }

    return 0;
}

// Message will look like "requestToBuyItem:key:cheese:amount:56"
int requestServerToBuyItem(const char *item_name, const char *amount, int server_port) {
    char message_to_send[strlen("requestToBuyItem:key:") + strlen(item_name) + strlen(":") + strlen("amount:") + strlen(amount)];
    memset(message_to_send, 0, strlen(message_to_send));
    strcat(message_to_send, "requestToBuyItem:key:");
    strcat(message_to_send, item_name);
    strcat(message_to_send, ":");
    strcat(message_to_send, "amount:");
    strcat(message_to_send, amount);

    int talking_status = talkToServer(server_port, message_to_send, sizeof(message_to_send));

    if (talking_status == atoi(ITEM_BUYING_SUCCESS)) {
        printf("[+] Customer was able to buy %s pieces of item '%s'\n", amount, item_name);
    } else if (talking_status == NETWORK_FAILURE) {
        printf("[-] Customer was unable to talk to server on port %d and therefore, "
               "unable to buy item '%s' from the store (This is due to a failed connection to the server)\n", server_port, item_name);
    } else if (talking_status == atoi(ITEM_BUYING_FAILURE)) {
        printf("[-] Customer was unable to buy %s pieces of item '%s' because the store doesn't have enough of them in stock\n", amount, item_name);
    } else if (talking_status == atoi(ITEM_DOESNT_EXIST)) {
        printf("[-] Customer was unable to buy %s pieces of item '%s' because the item doesn't exist in the store\n", amount, item_name);
    }

    return talking_status;
}

int main() {
    //interactiveMode();
    //automaticMode();
    handleConnection(AUTOMATIC_MODE);
}
