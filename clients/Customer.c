//
// Created by Aimen Fahmi on 2020-03-29.
//

#include "Customer.h"

#include "../util/client_utilities/ClientUtilities.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

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

    if (talking_status < 0) {
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

    if (talking_status == -1) {
        printf("[-] Client was unable to talk to server on port %d and therefore, "
               "unable to buy item '%s' from the store (This is due to a failed connection to the server)\n", server_port, item_name);
        return -1;
    } else if (talking_status == -2) {
        printf("[-] Client was unable to buy item '%s' from the store because the latter doesn't have enough of that item\n", item_name);
        return -2;
    }

    return 0;
}

int main() {
    int server_port = 9005;

    while (requestServerToBuyItem("chocolate cake", "2", server_port) == 0) {
        sleep(1);
    }

    requestServerToWriteItemToStore("fanta", "6", server_port);

    talkToServer(server_port, "exit", sizeof("exit"));
    
}
