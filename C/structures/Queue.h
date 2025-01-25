//
// Created by Alex on 25/01/2025.
//

#ifndef QUEUE_H
#define QUEUE_H

#include "List.h"

typedef struct QueueElem {
    iList* covered_vertices;
    iList* used_vertices;
    struct QueueElem* next;
} QueueElem;

typedef struct Queue {
    QueueElem* head;
    QueueElem* tail;
    int size;
} Queue;

Queue* queue_create();
void queue_add(Queue* queue, iList* l1, iList* l2);
QueueElem* queue_pop(Queue* queue);

#endif //QUEUE_H
