//
// Created by Aimen Fahmi on 2020-03-29.
//

#ifndef E_COMMERCE_STORE_CUSTOMER_H
#define E_COMMERCE_STORE_CUSTOMER_H

/** functions **/

void interactiveMode(int client_socket);
void automaticMode(int client_socket, int customer_id);
void handleConnection(int mode, int customer_id);
int requestServerToWriteItem(const char *item_name, const char *amount, int item_name_size, int amount_size, int client_socket);
int requestServerToBuyItem(const char *item_name, const char *amount, int item_name_size, int amount_size, int client_socket);

#endif //E_COMMERCE_STORE_CUSTOMER_H
