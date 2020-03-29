//
// Created by Aimen Fahmi on 2020-03-29.
//

#include "RegularClient.h"
#include "../util/client_utilities/ClientUtilities.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

// Message will look like: "key:cheese//amount:56"
int requestServerToAddItem(const char *item_name, const char *amount, int server_port) {
    char message_to_send[strlen("key:") + strlen(item_name) + strlen(":") + strlen("amount:") + strlen(amount)];
    memset(message_to_send, 0, strlen(message_to_send));
    strcat(message_to_send, "key:");
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

int getLine (char *prompt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prompt != NULL) {
        printf ("%s", prompt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}

int main() {

    unsigned int server_port = 9005;

    requestServerToAddItem("a drink", "58", server_port);
    requestServerToAddItem("some food", "200", server_port);
    requestServerToAddItem("chocolate cake", "58", server_port);
    requestServerToAddItem("lemon juice", "100", server_port);
    requestServerToAddItem("socks", "500", server_port);
    talkToServer(server_port, "exit", strlen("exit"));

    return 0;
}
