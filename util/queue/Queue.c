//
// Created by Aimen Fahmi on 2020-04-08.
//

#include "Queue.h"

node_t *tail = NULL;
node_t *head = NULL;

void enqueue(int *value) {
    node_t *newnode = malloc(sizeof(node_t));
    newnode->value = value;
    newnode->next = NULL;
    if (tail == NULL) {
        head = newnode;
    } else {
        tail->next = newnode;
    }
    tail = newnode;
}

int *dequeue() {
    if (head == NULL) {
        return NULL;
    } else {
        int * result = head->value;
        node_t *temp = head;
        head = head->next;
        if (head == NULL) {tail = NULL;}
        free(temp);
        return result;
    }
}
