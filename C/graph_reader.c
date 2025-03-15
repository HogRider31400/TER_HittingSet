//
// Created by Alex on 25/01/2025.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "graph_reader.h"

#include <string.h>

#include "HGraph.h"
/* This code is public domain -- Will Hartung 4/9/09 */
// https://stackoverflow.com/questions/735126/are-there-alternate-implementations-of-gnu-getline-interface/735472#735472
typedef intptr_t ssize_t;

ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    size_t pos;
    int c;

    if (lineptr == NULL || stream == NULL || n == NULL) {
        errno = EINVAL;
        return -1;
    }

    c = getc(stream);
    if (c == EOF) {
        return -1;
    }

    if (*lineptr == NULL) {
        *lineptr = malloc(128);
        if (*lineptr == NULL) {
            return -1;
        }
        *n = 128;
    }

    pos = 0;
    while(c != EOF) {
        if (pos + 1 >= *n) {
            size_t new_size = *n + (*n >> 2);
            if (new_size < 128) {
                new_size = 128;
            }
            char *new_ptr = realloc(*lineptr, new_size);
            if (new_ptr == NULL) {
                return -1;
            }
            *n = new_size;
            *lineptr = new_ptr;
        }

        ((unsigned char *)(*lineptr))[pos ++] = c;
        if (c == '\n') {
            break;
        }
        c = getc(stream);
    }

    (*lineptr)[pos] = '\0';
    return pos;
}

iList* parse_line(char *line, int size) {
    iList* list = create_list();

    int cur_nb = 0;

    for (int i = 0; i < size; i++) {
        if (line[i] == '\n') break;
        //Si c'est un espace on reset et on ajoute le nombre construit à la liste
        if (line[i] == ' ') {
            append(list,cur_nb);
            cur_nb = 0;
        }
        else {
            //Ce n'est pas un espace donc à priori c'est un chiffre, on vérifie bien que c'est un chiffre
            //puis on l'ajoute au nombre construit
            int cur_digit = line[i] - '0';

            cur_nb = cur_nb*10 +  cur_digit;
            //printf("char : %c, digit %d, cur_nb %d \n",line[i],cur_digit,cur_nb);
        }
    }
    if (cur_nb != 0) append(list,cur_nb);
    return list;
}

/*
 * Le format du contenu du fichier qui est attendu pour cette fonction est le suivant :
 *  - Chaque ligne est une hyper arête
 *  - Le contenu de chaque ligne est une suite de nombres séparés par un espace entre chaque nombre
 *
 *  Par exemple ceci est une hyper arête valide :
 *  1 2 3 4, qui va être comptées comme une hyper arête de taille 4 avec 4 sommets (d'id 1 2 3 et 4)
 *  On peut alors avoir l'hypergraphe suivant :
 *  1 2
 *  3 4
 *  1 4 3
 *
 *  Qui est un hypergraphe à 3 hyper arêtes et 4 sommets
 */
Graph* read_graph_from_file(char *filename) {

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("peut pas lire %s \n", filename);
        return NULL;
    }
    //printf("On a lu\n");

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    Graph* graph = create_graph();

    while ((read = getline(&line, &len, file)) != -1) {
        iList* line_list = parse_line(line, read);
        //print_list(line_list);
        //printf("\n");
        Edge* edge = create_edge();

        edge->id = graph->nb_edges+1;
        edge->vertices = line_list;
        add_edge(graph, edge);
    }

    fclose(file);
    if (line)
        free(line);
    //printf("Fichier lu %d\n",graph->nb_edges);
    for (int i = 0; i < graph->nb_edges; i++) {
        //printf("%d ", graph->edges[i]->id);
        Edge* edge = graph->edges[i];

        for (Node* cur = edge->vertices->head; cur != NULL; cur = cur->next) {
            //printf("%d ", cur->value);
            int id = cur->value - 1;

            if (graph->vertices[id] == NULL) {
                graph->vertices[id] = create_vertex();
                graph->vertices[id]->id = cur->value;
                graph->nb_vertices++;
            }
            append(graph->vertices[id]->edges, edge->id);
        }
    }

    return graph;
}