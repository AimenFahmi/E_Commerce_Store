//
// Created by Aimen Fahmi on 2020-03-29.
//

#include "Shop.h"
#include "../util/client_utilities/ClientUtilities.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>


// Message will look like: "increaseCountOfItem:key:cheese:amount:56:"
int requestServerToIncreaseCountOfItem(const char *item_name, const char *amount, int server_port) {
    char message_to_send[strlen("increaseCountOfItem:key:") + strlen(item_name) + strlen(":") + strlen("amount:") + strlen(amount)];
    memset(message_to_send, 0, strlen(message_to_send));
    strcat(message_to_send, "increaseCountOfItem:key:");
    strcat(message_to_send, item_name);
    strcat(message_to_send, ":");
    strcat(message_to_send, "amount:");
    strcat(message_to_send, amount);

    int talking_status = talkToServer(server_port, message_to_send, sizeof(message_to_send));

    if (talking_status < 0) {
        printf("[-] Client was unable to talk to server at port %d", server_port);
    } else if (strcmp(message_to_send, "exit") == 0) {
        printf("[+] Connection terminated\n");
    }
    return 0;
}

// Message will look like: "writeItemToStore:key:cheese:amount:56:"
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
        printf("[-] Client was unable to talk to server at port %d", server_port);
    } else if (strcmp(message_to_send, "exit") == 0) {
        printf("[+] Connection terminated\n");
    }
    return 0;
}

int main() {

    unsigned int server_port = 9005;

    requestServerToWriteItemToStore("a drink", "3", server_port);
    requestServerToWriteItemToStore("some food", "7", server_port);

    requestServerToIncreaseCountOfItem("a drink", "3", server_port);
    sleep(1);
    requestServerToIncreaseCountOfItem("some food", "5", server_port);
    sleep(1);
    requestServerToIncreaseCountOfItem("chocolate cake", "34", server_port);
    sleep(1);
    requestServerToIncreaseCountOfItem("lemon juice", "2", server_port);
    sleep(1);
    requestServerToIncreaseCountOfItem("socks", "9", server_port);
    sleep(1);

    talkToServer(server_port, "exit", strlen("exit"));

    return 0;
}
