//
// Created by Alex on 12/03/2025.
//

#include "Bitmap.h"

#include <stdio.h>
#include <stdlib.h>

bitmap* create_bitmap(int capacity) {
    bitmap* c_bitmap = malloc(sizeof(bitmap));
    c_bitmap->size = capacity;
    c_bitmap->bits = malloc(sizeof(int) * c_bitmap->size);
    c_bitmap->nb_bits = 0;
    for (int i = 0; i < c_bitmap->size; i++) {
        c_bitmap->bits[i] = 0;
    }

    return c_bitmap;
}

void free_bitmap(bitmap* c_bitmap) {
    free(c_bitmap->bits);
    free(c_bitmap);
}

int get_bit(bitmap* c_bitmap, int i) {
    //printf("aaa%d %daaa\n", i/(sizeof(int)*8), i%(sizeof(int)*8));
    return (c_bitmap->bits[i/(sizeof(int)*8)] >> (i%(sizeof(int)*8)) & 1);
}

void set_bit(bitmap* c_bitmap, int i, int value) {
    int tab_pos = i/(sizeof(int)*8);
    int bit_pos = i%(sizeof(int)*8);
    int val = c_bitmap->bits[tab_pos] >> bit_pos & 1;
    //printf("On veut set a %d, on modif le %d ieme byte a l'indice %d qui a comme val %d et on met %d \n", i, tab_pos, bit_pos, val, value);
    if (val == 0 && value == 1) {
        c_bitmap->bits[tab_pos] |= (1 << bit_pos);
        c_bitmap->nb_bits++;
    }
    if (val == 1 && value == 0) {
        c_bitmap->bits[tab_pos] &= ~(1 << bit_pos);
        c_bitmap->nb_bits--;
    }
}

void print_bitmap(bitmap* c_bitmap) {
    for (int i = 0; i < c_bitmap->size; i++) {
        for (int j = 0; j < sizeof(int)*8; j++) {
            printf("%d", get_bit(c_bitmap, i + j));
        }

        //printf("%d", c_bitmap->bits[i]);
    }
    printf(" : %d \n", c_bitmap->nb_bits);
}

int count_bits(bitmap* c_bitmap) {
    int count = 0;
    for (int i = 0; i < c_bitmap->size; i++) {
        for (int j = 0; j < sizeof(int)*8; j++) {
            if (get_bit(c_bitmap, i + j)) {
                count++;
            }
        }
    }
    return count;
}

//ATTENTION PAR CONVENTION IMPOSEE ICI B1 >= B2 EN TAILLE ATTENTIOOOON
bitmap* bitmap_and(bitmap* b1, bitmap* b2) {
    bitmap* c_bitmap = create_bitmap(b1->size);
    for (int i = 0; i < b2->size; i++) {
        c_bitmap->bits[i] = b2->bits[i] & b1->bits[i];
    }
    //c_bitmap->nb_bits = count_bits(c_bitmap);

    return c_bitmap;
}
bitmap* bitmap_or(bitmap* b1, bitmap* b2) {
    bitmap* c_bitmap = copy_bitmap(b1);
    for (int i = 0; i < b2->size; i++) {
        c_bitmap->bits[i] = b2->bits[i] | b1->bits[i];
    }
    //c_bitmap->nb_bits = count_bits(c_bitmap);

    return c_bitmap;
}

bitmap_list* create_bitmap_list() {
    bitmap_list* c_bitmap_list = (bitmap_list*)malloc(sizeof(bitmap_list));
    c_bitmap_list->size = 0;
    c_bitmap_list->head = NULL;
    return c_bitmap_list;
}

void add_bitmap(bitmap_list* c_bitmap_list, bitmap* c_bitmap) {
    if (c_bitmap_list->size == 0) {
        c_bitmap_list->size = 1;
        c_bitmap_list->head = malloc(sizeof(bitmap_node));
        c_bitmap_list->head->next = NULL;
        c_bitmap_list->head->value = c_bitmap;
    }
    else {
        c_bitmap_list->size++;
        bitmap_node* temp = c_bitmap_list->head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = malloc(sizeof(bitmap_node));
        temp->next->next = NULL;
        temp->next->value = c_bitmap;
    }
}

bitmap* copy_bitmap(bitmap* b) {
    bitmap* new_bitmap = malloc(sizeof(bitmap));
    new_bitmap->size = b->size;
    new_bitmap->nb_bits = b->nb_bits;
    new_bitmap->bits = malloc(sizeof(int) * new_bitmap->size);
    for (int i = 0; i < new_bitmap->size; i++) {
        new_bitmap->bits[i] = b->bits[i];
    }
    return new_bitmap;
}

void print_bitmaps(bitmap_list* c_bitmap_list) {
    bitmap_node* temp = c_bitmap_list->head;
    while (temp != NULL) {
        print_bitmap(temp->value);
        temp = temp->next;
    }
}

void swap_bitmaps(bitmap_node* b1, bitmap_node* b2) {
    bitmap* temp = b1->value;
    b1->value = b2->value;
    b2->value = temp;
}

int is_bequal(bitmap* b1, bitmap* b2) {
    if (b1->size != b2->size) {
        return 0;
    }
    for (int i = 0; i < b1->size; i++) {
        if (b1->bits[i] != b2->bits[i]) {
            return 0;
        }
    }
    return 1;
}

//On check si b2 est un sous ensemble de b1
int is_bsubset(bitmap* b1, bitmap* b2) {
    if (b1 == NULL || b2 == NULL) {
        return 0;
    }
    if (b1->size != b2->size) {
        return 0;
    }
    if (b1->bits == NULL || b2->bits == NULL) {
        return 0;
    }

    //printf("On va check si c'est subset\n");
    //print_bitmap(b1);
    //print_bitmap(b2);
    bitmap* and_res = bitmap_and(b1, b2);
    //print_bitmap(and_res);
    //printf("\n");
    if (is_bequal(and_res,b2)) {
        free_bitmap(and_res);
        return 1;
    }
    free_bitmap(and_res);
    return 0;
}

int has_bsubset(bitmap_list* b1, bitmap* b2) {
    bitmap_node* cur_node = b1->head;
    while (cur_node != NULL) {
        bitmap* cur = cur_node->value;
        if (is_bsubset(b2,cur)) {
            return 1;
        }
        cur_node = cur_node->next;
    }
    return 0;
}

void print_as_ints(bitmap_list* c_bitmap_list) {
    bitmap_node* temp = c_bitmap_list->head;
    while (temp != NULL) {
        bitmap* cur = temp->value;

        iList* nbs = create_list();

        for (int i = 0; i < cur->size*sizeof(int)*8; i++) {
            if (get_bit(cur, i) == 1) {
                append(nbs, i+1);
            }
        }
        //printf("slt : ");
        print_list(nbs);
        printf("\n");
        free_list(nbs);
        temp = temp->next;
    }
}

void bubble_sort_bitmap(bitmap_list* list) {
    if (list == NULL) return;
    //printf("CICICICI\n");
    for (int cur_iter = 0; cur_iter < list->size; cur_iter++) {
        //print_list_list(list);
        bitmap_node* current = list->head;
        bitmap_node* previous = NULL;
        while (current != NULL) {
            if (previous != NULL) {
                if (previous->value->nb_bits > current->value->nb_bits) {
                    swap_bitmaps(previous, current);
                }
            }

            previous = current;
            current = current->next;
        }

    }

}