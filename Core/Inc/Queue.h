#ifndef QUEUE_H
#define QUEUE_H

#include "Node.h"
#include "Cell.h"

typedef struct
{
    Node* head;
    Node* tail;
    int length;
} Queue;

Cell* pop(Queue* queue);
void push(Queue* queue, Cell* new_node_cell);

#endif
