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

    char message_to_send[100] = {0};
    unsigned int server_port = 9005;

    while (1) {
        getLine("Write message to server -> ", message_to_send, sizeof(message_to_send));
        printf("\n");
        if (strcmp(message_to_send, "exit") == 0) {
            printf("[+] Connection terminated\n");
            break;
        } else {
            int talking_status = talkToServer(server_port, message_to_send, strlen(message_to_send));
            if (talking_status < 0) {
                printf("[-] Client was unable to talk to server at port %d", server_port);
                break;
            }
        }
    }

    return 0;
}
