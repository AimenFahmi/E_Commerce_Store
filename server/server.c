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
#include "../util/random_utilities/CommunicationProtocol.h"


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


void sendAcknowledgment(int client_socket, char *acknowledgment, int acknowledgment_size) {
    send(client_socket, acknowledgment, acknowledgment_size, 0);
}

// Parses the clients messages and takes the actual actions requested by the client
int handleMessageReception(char *message, int message_size,int client_socket) {
    if (strcmp(message, "exit") == 0) {
        printf("[+] Server has become disconnected\n");
        return SERVER_DISCONNECT;
    } else {
        request_t *request = parseClientMessage(message, message_size);
        const char *key = request->keyValuePair->key;
        value_t *value = request->keyValuePair->value;

        if (strcmp(request->command, "increaseCountOfItem") == 0) {

            if (increaseCountOfItem(key, value->nb_items, store) == -1) {
                printf("[-] Shop %d was unable to increase the count of item '%s' by %d "
                       "because the count of that item is already too big\n", client_socket, key, value->nb_items);
                sendAcknowledgment(client_socket, ITEM_COUNT_INCREASE_FAILURE, sizeof(ITEM_COUNT_INCREASE_FAILURE));
                return 0;
            } else {
                sendAcknowledgment(client_socket, ITEM_COUNT_INCREASE_SUCCESS, sizeof(ITEM_COUNT_INCREASE_SUCCESS));
                printf("[+] Count of item %s has been increased by %d\n", key, value->nb_items);
            }

        } else if (strcmp(request->command, "writeItemToStore") == 0) {
            writeItemToStore(key, value->nb_items, store);
            sendAcknowledgment(client_socket, ITEM_WRITING_SUCCESS, sizeof(ITEM_WRITING_SUCCESS));
            printf("[+] %d items of type '%s' have been written to the store\n", value->nb_items, key);

        } else if (strcmp(request->command, "requestToBuyItem") == 0) {
            int request_status = requestItem(key, value->nb_items, store);
            if (request_status == -1) {
                printf("[-] Customer %d was unable to buy item '%s' due to a lack of that item in the store\n", client_socket, key);
                sendAcknowledgment(client_socket, ITEM_BUYING_FAILURE, sizeof(ITEM_BUYING_FAILURE));
                return 0;
            } else if (request_status == -2) {
                printf("[-] Customer %d was unable to buy item '%s' because it doesn't exist in the store\n", client_socket, key);
                sendAcknowledgment(client_socket, ITEM_DOESNT_EXIST, sizeof(ITEM_DOESNT_EXIST));
            } else {
                printf("[+] %d items of type '%s' have been bought from the store\n", value->nb_items, key);
                sendAcknowledgment(client_socket, ITEM_BUYING_SUCCESS, sizeof(ITEM_BUYING_SUCCESS));
            }

        }
    }

    return 0;
}

// Executes the clients request and sends an acknowledgment
void * handleConnection(void *p_client_socket) {
    char message_to_receive[100];
    int *return_value = malloc(sizeof(int));
    memset(message_to_receive, 0, sizeof(message_to_receive));

    int client_socket = * ((int *) p_client_socket);

    while (1) {
        int message_reception_status = recv(client_socket, message_to_receive, sizeof(message_to_receive), 0);

        if (message_reception_status < 0) {
            printf("[-] Could not receive message from client on port %d\n", PORT);
            *return_value = -1;
            return return_value;
        }

        if (strcmp(message_to_receive, "exit") == 0) {
            *return_value = SERVER_DISCONNECT;
            break;
        }

        *return_value = handleMessageReception(message_to_receive, sizeof(message_to_receive), client_socket);

    }

    return return_value;
}

// Enables the server to listen to client requests and perform the corresponding actions
int talkToClients() {

    int socket = createServerSocket();
    bindCreatedSocket(socket, PORT);
    listen(socket, 90);

    printf("[+] Waiting for connections...\n");

    while (1) {
        int * client_socket = malloc(sizeof(int));
        *client_socket = accept(socket, NULL, NULL);

        if (client_socket < 0) {
            printf("[-] Could not accept client on port %d\n", PORT);
            return -1;
        }

        if (* ((int *) handleConnection(client_socket)) == SERVER_DISCONNECT) {
            close(*client_socket);
            close(socket);
            free(client_socket);
            break;
        }
    }

    return 0;
}

int main() {

    store = createStore();

    displayStore(store);

    talkToClients();

    displayStore(store);

    return 0;
}