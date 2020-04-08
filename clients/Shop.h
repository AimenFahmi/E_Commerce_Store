//
// Created by Aimen Fahmi on 2020-03-29.
//

#ifndef E_COMMERCE_STORE_SHOP_H
#define E_COMMERCE_STORE_SHOP_H

void automaticMode(int client_socket);
void handleConnection(int mode);
int requestServerToIncreaseCountOfItem(const char *item_name, const char *amount, int item_name_size, int amount_size, int client_socket);

#endif //E_COMMERCE_STORE_SHOP_H
