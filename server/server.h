//
// Created by Aimen Fahmi on 2020-03-29.
//

#ifndef E_COMMERCE_STORE_SERVER_H
#define E_COMMERCE_STORE_SERVER_H

#include "../hash_table/HashTable.h"
#include "store.h"
#include "store.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "../util/server_utilities/ServerUtilities.h"
#include "../util/exception_handling/Failure.h"
#include "../util/random_utilities/CommunicationProtocol.h"
#include "../util/queue/Queue.h"
#include "../util/random_utilities/RandomUtilities.h"

/** data **/

#define SERVER_DISCONNECT -1
#define THREAD_POOL_SIZE 100
pthread_t thread_pool[THREAD_POOL_SIZE];

pthread_mutex_t store_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_variable = PTHREAD_COND_INITIALIZER;

int server_socket;

typedef struct {
    char *command;
    key_value_pair_t *keyValuePair;
} request_t;

store_t *store;

/** functions **/

void sigintHandler(int num);
void *thread_function(void * arg);
void talkToClients();
int handleMessageReception(char *message, int message_size,int client_socket);
request_t *parseClientMessage(char *received_message, int message_size);
char **tokenize(char *string, int string_size, char *delimiter);
void sendAcknowledgment(int client_socket, char *acknowledgment, int acknowledgment_size);

#endif //E_COMMERCE_STORE_SERVER_H
