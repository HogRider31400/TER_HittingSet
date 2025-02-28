//
// Created by Alex on 25/01/2025.
//

#include "Queue.h"

#include <stdio.h>
#include <stdlib.h>

Queue* queue_create() {
    Queue* queue = malloc(sizeof(Queue));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void queue_add(Queue* queue, iList* covered_vertices, iList* used_vertices, int mask) {
    QueueElem* new_elem = malloc(sizeof(QueueElem));
    new_elem->covered_edges = covered_vertices;
    new_elem->used_vertices = used_vertices;
    new_elem->next = NULL;
    new_elem->used_mask = mask;
    if (queue->tail == NULL) {
        queue->head = new_elem;
        queue->tail = new_elem;
    }
    else {
        queue->tail->next = new_elem;
        queue->tail = new_elem;
    }
}
QueueElem* queue_pop(Queue* queue) {
    if (queue->head == NULL) {
        return NULL;
    }
    QueueElem* pop_elem = queue->head;
    queue->head = pop_elem->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    return pop_elem;
}
QueueA* queue_a_create() {
    QueueA* queue = malloc(sizeof(QueueA));
    queue->elems = malloc(DEFAULT_SIZE * sizeof(QueueAElem));
    queue->cur_size = 0;
    queue->head = 0;
    queue->tail = 0;
    queue->max_size = DEFAULT_SIZE;
    return queue;
}

void queue_a_add(QueueA* queue, iList* covered_vertices, iList* used_vertices) {
    QueueAElem* new_elem = malloc(sizeof(QueueAElem));
    new_elem->covered_vertices = deep_copy(covered_vertices);
    new_elem->used_vertices = deep_copy(used_vertices);
    printf("%d\n", queue->cur_size);
    if (queue->cur_size + 1 >= queue->max_size) {
        queue->elems = realloc(queue->elems, 2 * queue->max_size * sizeof(QueueAElem));
        queue->max_size *= 2;
    }

    queue->elems[queue->tail] = *new_elem;
    //free(new_elem);

    queue->tail = (queue->tail + 1) % queue->max_size;
    queue->cur_size++;
}

QueueAElem queue_a_pop(QueueA* queue) {
    if (queue->cur_size == 0) {
        QueueAElem empty = {NULL, NULL};
        return empty;
    }

    QueueAElem pop_elem = queue->elems[queue->head];
    queue->head = (queue->head + 1) % queue->max_size;
    queue->cur_size--;
    return pop_elem;
}