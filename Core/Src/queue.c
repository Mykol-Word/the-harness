#include "stdlib.h"

#include "Queue.h"
#include "Node.h"
#include "Cell.h"

Cell* pop(Queue* queue)
{
    //If there is a head, set the head to its child, return the cell it contains, destroy it.
    if (queue->head)
    {
        Cell* popped_cell = queue->head->value;
        Node* new_head = queue->head->child;

        free(queue->head);
        queue->head = new_head;
        if (!new_head) queue->tail = new_head;

        return popped_cell;
    }

    //If no head, return nullptr.
    return 0;
}

void push(Queue* queue, Cell* new_node_cell)
{
    //Create new node with cell
    Node* new_node = malloc(sizeof(Node));
    new_node->child = 0;
    new_node->value = new_node_cell;

    //If the list isn't empty
    if (queue->tail)
    {
        queue->tail->child = new_node;
        queue->tail = new_node;
    }
    //If the list is empty
    else
    {
        queue->tail = new_node;
        queue->head = new_node;
    }
}
