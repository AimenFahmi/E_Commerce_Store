//
// Created by Aimen Fahmi on 2020-03-29.
//

#include "server.h"
#include "store.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

#include "../util/server_utilities/ServerUtilities.h"

/*C substring function: It returns a pointer to the substring */

char *substring(char *string, int position, int length)
{
    char *pointer;
    int c;

    pointer = malloc(length+1);

    if (pointer == NULL)
    {
        printf("Unable to allocate memory.\n");
        exit(1);
    }

    for (c = 0 ; c < length ; c++)
    {
        *(pointer+c) = *(string+position-1);
        string++;
    }

    *(pointer+c) = '\0';

    return pointer;
}

key_value_pair_t *parseClientMessage(char *received_message, int message_length, char delimiter) {

    char *key;
    int amount;

    int key_length = 0;
    int amount_length = 0;

    // get length of the key
    for (int i = 4; i < message_length; ++i) {
        if (received_message[i] == delimiter) break;
        key_length += 1;
    }

    key = substring(received_message, 5, key_length);

    // get length of the amount

    amount_length = message_length - 5 - key_length - 7;

    amount = atoi(substring(received_message, 5 + key_length + 8, amount_length));

    return createKeyValuePair(key, createValue(amount));
}

int handleMessageReception(char *message, int client_socket, store_t *store) {
    if (strcmp(message, "exit") == 0) {
        close(client_socket);
        return -1;
    } else {
        key_value_pair_t *keyValuePair = parseClientMessage(message, strlen(message), ':');
        addItem(keyValuePair->key, keyValuePair->value->nb_items, store);

        printf("[+] Message from client: 'I want you to add %d items of type '%s''\n",
               keyValuePair->value->nb_items, keyValuePair->key);
    }
    return 0;
}

int talkToClient(unsigned int client_port, store_t *store) {
    char message_to_receive[100] = {0};

    int socket = createServerSocket();
    bindCreatedSocket(socket, client_port);
    listen(socket, 5);

    while (1) {
        memset(message_to_receive, 0, sizeof(message_to_receive));

        int client_socket = accept(socket, NULL, NULL);

        if (client_socket < 0) {
            printf("[-] Could not accept client on port %d\n", client_port);
            return -1;
        }

        int message_reception_status = recv(client_socket, message_to_receive, sizeof(message_to_receive), 0);

        if (message_reception_status < 0) {
            printf("[-] Could not receive message from client on port %d\n", client_port);
            return -1;
        }

        if (handleMessageReception(message_to_receive, client_socket, store) == -1) break;

    }

    printf("[+] Server has become disconnected\n");
    return 0;
}

int main() {

    store_t *store = createStore();

    displayStore(store);

    talkToClient(9005, store);

    displayStore(store);

    return 0;
}