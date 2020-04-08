//
// Created by Aimen Fahmi on 2020-03-29.
//

#ifndef E_COMMERCE_STORE_SERVERUTILITIES_H
#define E_COMMERCE_STORE_SERVERUTILITIES_H


int createServerSocket();
int bindCreatedSocket(int socket, unsigned int client_port);
void sendAcknowledgment(int client_socket, char *acknowledgment, int acknowledgment_size);

#endif //E_COMMERCE_STORE_SERVERUTILITIES_H
