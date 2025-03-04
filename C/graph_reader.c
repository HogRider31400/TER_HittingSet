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

size_t getline(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while(c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
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
        return NULL;
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    Graph* graph = create_graph();

    while ((read = getline(&line, &len, file)) != -1) {
        iList* line_list = parse_line(line, read);
        Edge* edge = create_edge();
        edge->id = graph->nb_edges+1;
        edge->vertices = line_list;
        add_edge(graph, edge);
    }

    fclose(file);
    if (line)
        free(line);

    for (int i = 0; i < graph->nb_edges; i++) {
        Edge* edge = graph->edges[i];

        for (Node* cur = edge->vertices->head; cur != NULL; cur = cur->next) {

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