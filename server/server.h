//
// Created by Aimen Fahmi on 2020-03-29.
//

#ifndef E_COMMERCE_STORE_SERVER_H
#define E_COMMERCE_STORE_SERVER_H

#include "../hash_table/HashTable.h"
#include "store.h"

/** data **/

#define UNABLE_TO_INCREASE_COUNT_ERROR -2
#define DISCONNECT -3
#define UNABLE_TO_BUY_ITEM_ERROR -4

#define PROBLEM "1"
#define SUCCESS "0"

typedef struct {
    char *command;
    key_value_pair_t *keyValuePair;
} request_t;

/** functions **/

int talkToClient(unsigned int client_port, store_t *store);
int handleMessageReception(char *message, int message_size,int client_socket, store_t *store);
request_t *parseClientMessage(char *received_message, int message_size);
char **tokenize(char *string, int string_size, char *delimiter);

#endif //E_COMMERCE_STORE_SERVER_H
