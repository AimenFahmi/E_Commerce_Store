//
// Created by Aimen Fahmi on 2020-03-29.
//

#ifndef E_COMMERCE_STORE_CUSTOMER_H
#define E_COMMERCE_STORE_CUSTOMER_H

/** functions **/

int requestServerToWriteItemToStore(const char *item_name, const char *amount, int server_port);
int requestServerToBuyItem(const char *item_name, const char *amount, int server_port);

#endif //E_COMMERCE_STORE_CUSTOMER_H
