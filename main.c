#include <stdio.h>

#include "server/store.h"
#include "util/server_utilities/ServerUtilities.h"

int main() {
    // Creating and initializing the store randomly after every execution
    /*store_t *store = createStore();

    displayStore(store);

    // Adding a not already existing amount of items to the store
    addItem("cashew nuts", 999, store);

    // Adding an existing amount of items to the store
    addItem("socks", 100, store);

    // Getting the available number of items in the store of a specific item
    printf("\n\nValue got: %d", getValue("cashew nuts", store->stock)->nb_items);


    // Requesting an amount of items
    int status = requestItem("chocolate cake", 200, store);
    if (status == 0) printf("\n\nRequest was successful");
    else printf("\n\nRequest was unsuccessful");


    displayStore(store);*/

    talkToClient(9005);

    return 0;
}