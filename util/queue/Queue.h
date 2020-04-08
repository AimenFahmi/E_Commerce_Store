//
// Created by Aimen Fahmi on 2020-04-08.
//

#ifndef E_COMMERCE_STORE_QUEUE_H
#define E_COMMERCE_STORE_QUEUE_H

#include <stdio.h>
#include <stdlib.h>

struct node{
    struct node *next;
    int *value;
} node;

typedef struct node node_t;

void enqueue(int *value);
int *dequeue();

#endif //E_COMMERCE_STORE_QUEUE_H
