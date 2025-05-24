#ifndef NODE_H
#define NODE_H

#include "Cell.h"

typedef struct
{
    struct Node* child;
    Cell* value;
} Node;

#endif
