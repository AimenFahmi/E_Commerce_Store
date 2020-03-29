//
// Created by Aimen Fahmi on 2020-03-27.
//

#include "store.h"
#include "../hash_table/HashTable.h"
#include <string.h>
#include <time.h>
#include "../util/exception_handling/Failure.h"


// Returns a random item name (obviously)
const char *getRandomItemName() {
    // 31 item choice
    const char *items[] = {"socks", "trainers", "tv", "computers", "bicycles", "Iphone X", "toilet paper", "Samsung Galaxy 10",
                     "noodles", "milk", "bread", "chocolate cake", "vanilla cake", "chocolate biscuits", "red bull",
                     "coca cola", "fanta", "ice tea", "lemon juice", "evian water", "walser water", "ramseier classic",
                     "ramseier shorlei", "chocolate milk", "chocolate yogurt", "vanilla yogurt", "cottage cheese",
                     "hot pepper sauce", "hot chili sauce", "white bread"};

    return items[rand()%30];
}

// Initializes the underlying data structure of the store randomly
void initializeStore(int size, store_t *store) {

    for (int i = 0; i < 5*size; ++i) {
        add(getRandomItemName(), createValue(rand()%500), store->stock);
    }
}


// Creates the hashtable that stores the items
store_t *createStore() {
    srand(time(0));

    store_t *newStore = malloc(sizeof(store_t));

    manageMallocFailures(newStore, "store_t");

    newStore->stock = createHashTable(STORE_SIZE);

    initializeStore(newStore->stock->size, newStore);

    return newStore;
}


// Quite lazy way of implementing it, but shouldn't need much more performance
// If there are enough items (the requested amount) the method returns 0
// Otherwise it returns -1
int requestItem(const char *itemName, int amount, store_t *store) {
    value_t *value = getValue(itemName, store->stock);

    if (value->nb_items - amount > 0) {
        value->nb_items -= amount;
        add(itemName, value, store->stock);
        return 0;
    } else {
        return -1;
    }
}

void addItem(const char *itemName, int amount, store_t *store) {
    value_t *current_value = getValue(itemName, store->stock);

    if (current_value == NULL) {
        add(itemName, createValue(amount), store->stock);
    } else {
        current_value->nb_items += amount;
        add(itemName, current_value, store->stock);
    }
}


// Displays the underlying data structure of the store
void displayStore(store_t *store) {
    printHashTable(store->stock);
}
