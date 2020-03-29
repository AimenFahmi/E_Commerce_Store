//
// Created by Aimen Fahmi on 2020-03-27.
//

#include "HashTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../util/exception_handling/Failure.h"

// Memory allocation for a hashtable
hash_table_t *createHashTable(int size) {

    hash_table_t *newHashTable = malloc(sizeof(hash_table_t));

    manageMallocFailures(newHashTable, "hash_table_t");

    newHashTable->entries = malloc(size * sizeof(key_value_pair_t *));

    manageMallocFailures(newHashTable->entries, "hash_table_t entries");

    for (int i = 0; i < size; ++i) {
        newHashTable->entries[i] = NULL;
    }

    newHashTable->size = size;

    return newHashTable;
}


// Helper for the createKeyValuePair
value_t *createValue(int value) {

    value_t *newValue = malloc(sizeof(value_t));

    manageMallocFailures(newValue, "value_t");

    newValue->nb_items = value;

    return newValue;
}


// Memory allocation for the given KeyValuePair
key_value_pair_t *createKeyValuePair(const char *key, value_t *value) {

    key_value_pair_t *newEntry = malloc(sizeof(key_value_pair_t));

    manageMallocFailures(newEntry, "key_value_pair_t");

    newEntry->value = value;
    newEntry->key = key;

    newEntry->next = NULL;

    return newEntry;
}


// Returns a hash code
int hash(const char *key, hash_table_t *hashTable) {

    unsigned int value = 0;
    int key_length = strlen(key);

    for (int i = 0; i < key_length; ++i) {
        value = value * 37 + key[i];
    }

    value = value % hashTable->size;

    return value;
}


// If the item wasn't in the hashtable already, it adds it
// else if overwrites it's value
void add(const char *key, value_t *value, hash_table_t *hashTable) {

    int index = hash(key, hashTable);

    key_value_pair_t *entry = hashTable->entries[index];

    if (entry == NULL) {
        hashTable->entries[index] = createKeyValuePair(key, value);
        return;
    }

    key_value_pair_t *prev;

    while (entry != NULL) {

        if (strcmp(entry->key, key) == 0) {
            memcpy(entry->value, value, sizeof(value_t));
            return;
        }
        prev = entry;
        entry = prev->next;

    }

    prev->next = createKeyValuePair(key, value);
}


// Searches for the key in the hashtable
// If found, it returns it's corresponding value
// else, it returns null
value_t *getValue(const char *key, hash_table_t *hashTable) {
    int index = hash(key, hashTable);
    key_value_pair_t *entry = hashTable->entries[index];

    if (entry == NULL) {
        printf("Item does not exit");
        return NULL;
    }

    if (strcmp(key, entry->key) == 0) {
        if (hashTable->entries[index]->value->nb_items != VALUE_AFTER_REMOVAL)
            return hashTable->entries[index]->value;
    }

    while (entry != NULL) {

        if (strcmp(key, entry->key) == 0) {
            if (hashTable->entries[index]->value->nb_items != VALUE_AFTER_REMOVAL)
                return entry->value;
        }

        entry = entry->next;
    }

    return NULL;
}


// Lazy delete by setting the nb_items of the Item to NULL
void removeItem(const char *key, hash_table_t *hashTable) {
    add(key, createValue(VALUE_AFTER_REMOVAL), hashTable);
}

// Helper for the printHashTable function
void printValue(value_t *value) {
    printf("%d ", value->nb_items);
}


// Prints the hashtable on the standard output
void printHashTable(hash_table_t *hashTable) {
    if (hashTable == NULL) {
        printf("Can't print a null hash table");
        exit(-1);
    }

    printf("\n\n");

    for (int i = 0; i < hashTable->size; ++i) {

        key_value_pair_t *entry = hashTable->entries[i];
        if (entry != NULL) {
            printf("at index %d: ", i);

            while (entry != NULL) {
                if (entry->value->nb_items != VALUE_AFTER_REMOVAL) {
                    printf("%s -> ", entry->key);
                    printValue(entry->value);
                }
                entry = entry->next;
            }

            printf("\n");
        }

    }
}


