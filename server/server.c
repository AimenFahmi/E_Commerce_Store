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
#include "../util/exception_handling/Failure.h"


// Tokenizes the string according to the delimiter and returns an array containing all the tokens
char **tokenize(char *string, int string_size, char *delimiter) {
    int number_of_tokens = 0;
    char *temp_string_1 = malloc(string_size);
    char *temp_string_2 = malloc(string_size);

    manageMallocFailures(temp_string_1, "variable in tokenize()");
    manageMallocFailures(temp_string_2, "variable in tokenize()");

    strcpy(temp_string_1, string);
    strcpy(temp_string_2, string);

    char *token = strtok(temp_string_1, delimiter);

    while (token != NULL) {
        number_of_tokens++;
        token = strtok(NULL, delimiter);
    }

    char **tokens = malloc(number_of_tokens * sizeof(char *));

    manageMallocFailures(tokens, "tokens in server.c/tokenize()");

    tokens[0] = strtok(temp_string_2, delimiter);

    for (int i = 1; i < number_of_tokens; ++i) {
        tokens[i] = strtok(NULL, delimiter);
    }

    return tokens;
}


// Returns the clients message in the form of a struct containing all the attributes of the message
// It is important to note that this function does not check whether the message is in the correct form or not !
request_t *parseClientMessage(char *received_message, int message_size) {

    request_t *request = malloc(sizeof(request_t));
    manageMallocFailures(request, "request in parseClientMessage()");

    char **tokens = tokenize(received_message, message_size, ":");

    request->command = tokens[0];
    
    char *key = tokens[2];
    int amount = atoi(tokens[4]);

    request->keyValuePair = createKeyValuePair(key, createValue(amount));

    return request;
}


// Parses the clients messages and takes the actual actions requested by the client
int handleMessageReception(char *message, int message_size,int client_socket, store_t *store) {
    if (strcmp(message, "exit") == 0) {
        printf("[+] Server has become disconnected\n");
        close(client_socket);
        return -3;
    } else {
        request_t *request = parseClientMessage(message, message_size);
        const char *key = request->keyValuePair->key;
        value_t *value = request->keyValuePair->value;

        if (strcmp(request->command, "increaseCountOfItem") == 0) {
            if (increaseCountOfItem(key, value->nb_items, store) == -1) {
                printf("[-] Shop %d was unable to increase the count of item '%s' by %d "
                       "because the count of that item is already too big\n", client_socket, key, value->nb_items);
                return UNABLE_TO_INCREASE_COUNT_ERROR;
            } else {
                printf("[+] Count of item %s has been increased by %d\n", key, value->nb_items);
            }

        } else if (strcmp(request->command, "writeItemToStore") == 0) {
            writeItemToStore(key, value->nb_items, store);
            printf("[+] %d items of type '%s' have been written to the store\n", value->nb_items, key);

        } else if (strcmp(request->command, "requestToBuyItem") == 0) {
            if (requestItem(key, value->nb_items, store) == -1) {
                printf("[-] Customer %d was unable to buy item '%s' due to a lack of that item in the store\n", client_socket, key);
                return UNABLE_TO_BUY_ITEM_ERROR;
            } else {
                printf("[+] %d items of type '%s' have been bought from the store\n", value->nb_items, key);
            }

        }
    }
    return 0;
}


// Enables the server to listen to client requests and perform the corresponding actions
int talkToClient(unsigned int client_port, store_t *store) {
    char message_to_receive[100];

    int socket = createServerSocket();
    bindCreatedSocket(socket, client_port);
    listen(socket, 90);

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

        int message_handling_status = handleMessageReception(message_to_receive, sizeof(message_to_receive), client_socket, store);

        // Sending the right acknowledgments to the client
        if (message_handling_status == DISCONNECT) {
            break;
        } else if (message_handling_status == UNABLE_TO_INCREASE_COUNT_ERROR) {
            send(client_socket, PROBLEM, sizeof(PROBLEM), 0);
        } else if (message_handling_status == UNABLE_TO_BUY_ITEM_ERROR) {
            send(client_socket, PROBLEM, sizeof(PROBLEM), 0);
        }

        send(client_socket, SUCCESS, sizeof(SUCCESS), 0);
    }

    return 0;
}

int main() {

    store_t *store = createStore();

    displayStore(store);

    talkToClient(9005, store);

    displayStore(store);

    return 0;
}