//
// Created by Aimen Fahmi on 2020-03-27.
//

#ifndef E_COMMERCE_STORE_STORE_H
#define E_COMMERCE_STORE_STORE_H

#include "../hash_table/HashTable.h"
#include <stdio.h>
#include <stdlib.h>

/** data **/

#define STORE_SIZE 100

typedef struct {
    hash_table_t *stock;
} store_t;


/** functions **/

void initializeStore(int size, store_t *store);
store_t *createStore();
void displayStore(store_t *store);
int requestItem(const char *itemName, int amount, store_t *store);
void addItem(const char *itemName, int amount, store_t *store);

#endif //E_COMMERCE_STORE_STORE_H
