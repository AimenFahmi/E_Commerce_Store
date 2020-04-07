//
// Created by Aimen Fahmi on 2020-03-29.
//

#ifndef E_COMMERCE_STORE_CLIENTUTILITIES_H
#define E_COMMERCE_STORE_CLIENTUTILITIES_H

#include "../random_utilities/CommunicationProtocol.h"

int createClientSocket();
int connectToServer(int socket, unsigned int server_port);
int sendRequest(int socket, char *request_message, int request_length);
int receiveResponse(int socket, char *to_store_response, int response_length);
int talkToServer(unsigned int server_port, char *message_to_send, int message_length);

#endif //E_COMMERCE_STORE_CLIENTUTILITIES_H
