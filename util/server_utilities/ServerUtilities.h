//
// Created by Aimen Fahmi on 2020-03-29.
//

#ifndef E_COMMERCE_STORE_SERVERUTILITIES_H
#define E_COMMERCE_STORE_SERVERUTILITIES_H

#define PORT 9005

int createServerSocket();
int bindCreatedSocket(int socket, unsigned int client_port);
int talkToClient(unsigned int client_port);

#endif //E_COMMERCE_STORE_SERVERUTILITIES_H
