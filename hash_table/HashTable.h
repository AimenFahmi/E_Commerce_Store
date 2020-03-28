//
// Created by Aimen Fahmi on 2020-03-27.
//

#ifndef E_COMMERCE_STORE_HASHTABLE_H
#define E_COMMERCE_STORE_HASHTABLE_H

#define VALUE_AFTER_REMOVAL 0

/** data **/

typedef struct {
    int nb_items;
} value_t;

typedef struct key_value_pair_t{
    const char *key;
    value_t *value;
    struct key_value_pair_t *next;
} key_value_pair_t;

typedef struct {
    int size;
    key_value_pair_t **entries;
} hash_table_t;

/** functions **/

hash_table_t *createHashTable(int size);
value_t *createValue(int value);
key_value_pair_t *createKeyValuePair(const char *key, value_t *value);
int hash(const char *key, hash_table_t *hashTable);
void add(const char *key, value_t *value, hash_table_t *hashTable);
value_t *getValue(const char *key, hash_table_t *hashTable);
void printValue(value_t *value);
void printHashTable(hash_table_t *hashTable);
void removeItem(const char *key, hash_table_t *hashTable);


#endif //E_COMMERCE_STORE_HASHTABLE_H
