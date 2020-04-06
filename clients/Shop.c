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

    if (talking_status == atoi(ITEM_COUNT_INCREASE_SUCCESS)) {
        printf("[+] Shop successfully increased count of item '%s' by %s\n", item_name, amount);
    } else if (talking_status == atoi(ITEM_COUNT_INCREASE_FAILURE)) {
        printf("[-] Shop was unable to increase count of item '%s' by %s because the store contains more than 3 pieces of that item\n", item_name, amount);
    } else if (talking_status == NETWORK_FAILURE) {
        printf("[-] Shop was unable to talk to server at port %d\n", server_port);
    }

    return 0;
}

int main() {

    requestServerToIncreaseCountOfItem("a drink", "3", PORT);
    requestServerToIncreaseCountOfItem("some food", "5", PORT);
    requestServerToIncreaseCountOfItem("chocolate cake", "34", PORT);
    requestServerToIncreaseCountOfItem("lemon juice", "2", PORT);
    requestServerToIncreaseCountOfItem("socks", "9", PORT);

    talkToServer(PORT, "exit", strlen("exit"));

    return 0;
}
