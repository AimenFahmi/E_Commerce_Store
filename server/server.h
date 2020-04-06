//
// Created by Aimen Fahmi on 2020-03-29.
//

#ifndef E_COMMERCE_STORE_SERVER_H
#define E_COMMERCE_STORE_SERVER_H

#include "../hash_table/HashTable.h"
#include "store.h"

/** data **/

#define SERVER_DISCONNECT -1

typedef struct {
    char *command;
    key_value_pair_t *keyValuePair;
} request_t;

store_t *store;

/** functions **/

int talkToClients();
int handleMessageReception(char *message, int message_size,int client_socket);
request_t *parseClientMessage(char *received_message, int message_size);
char **tokenize(char *string, int string_size, char *delimiter);
void sendAcknowledgment(int client_socket, char *acknowledgment, int acknowledgment_size);

#endif //E_COMMERCE_STORE_SERVER_H
