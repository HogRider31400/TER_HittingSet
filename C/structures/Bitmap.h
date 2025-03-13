//
// Created by Alex on 12/03/2025.
//

#ifndef BITMAP_H
#define BITMAP_H
#include <stdlib.h>
#include "List.h"

typedef struct bitmap {
    int size;
    int nb_bits;
    int* bits;
} bitmap;

typedef struct bitmap_node {
    bitmap* value;
    struct bitmap_node* next;
} bitmap_node;

typedef struct bitmap_list {
    int size;
    bitmap_node* head;
} bitmap_list;

bitmap* create_bitmap(int capacity);
void free_bitmap(bitmap* c_bitmap);
int get_bit(bitmap* c_bitmap, int i);
void set_bit(bitmap* c_bitmap, int i, int value);
bitmap* copy_bitmap(bitmap* b);
void print_bitmap(bitmap* c_bitmap);
bitmap* bitmap_and(bitmap* b1, bitmap* b2);
bitmap* bitmap_or(bitmap* b1, bitmap* b2);

bitmap_list* create_bitmap_list();
void add_bitmap(bitmap_list* c_bitmap_list, bitmap* c_bitmap);
void print_bitmaps(bitmap_list* c_bitmap_list);
int has_bsubset(bitmap_list* b1, bitmap* b2);
void bubble_sort_bitmap(bitmap_list* list);
void print_as_ints(bitmap_list* c_bitmap_list);
#endif //BITMAP_H
