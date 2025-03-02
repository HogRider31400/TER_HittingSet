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
    //printf("Liste parsée en ints : \n");
    //print_list(list);
    //printf("\n");
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
    //printf("ici\n");
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir %s\n", filename);
        return NULL;
    }
    //printf("ici\n");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    Graph* graph = create_graph();
    //printf("ici\n");
    while ((read = getline(&line, &len, file)) != -1) {
        //On parse et on ajoute l'hyper arête au graphe
        iList* line_list = parse_line(line,read);

        Edge* edge = create_edge();
        edge->id = graph->nb_edges+1;
        edge->vertices = line_list;
        add_edge(graph,edge);
    }

    fclose(file);
    if (line)
        free(line);

    //on init un tableau de correspondance pour que tout tienne sur un tableau
    //car sinon il y a des trous, beaucoup
    int corresp[MAX_VERTICES];
    memset(corresp, -1, sizeof(corresp));

    //On a toutes les hyper arêtes il faut reconstruire les sommets à partir de ça
    for (int i = 0; i < graph->nb_edges; i++) {
        Edge* edge = graph->edges[i];

        for (Node* cur = edge->vertices->head; cur != NULL; cur = cur->next) {
            if (corresp[cur->value-1] == -1) {
                corresp[cur->value-1] = graph->nb_vertices;
            }
            int id = corresp[cur->value-1];
            //printf("%d %d \n",cur->value-1,id);
            //On vérifie si le sommet a déjà été init, si non on le fait
            if (graph->vertices[id] == NULL) {
                graph->vertices[id] = create_vertex();
                graph->vertices[id]->id = cur->value;
                graph->nb_vertices++;
            }
            append(graph->vertices[id]->edges, edge->id);
        }

    }
    //printf("La sommet 3 a :");
    //print_list(graph->vertices[2]->edges);
    //printf("\n");

    return graph;

}
a_Graph* read_agraph_from_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir %s\n", filename);
        return NULL;
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    a_Graph* graph = create_agraph();

    while ((read = getline(&line, &len, file)) != -1) {
        iList* line_list = parse_line(line, read);

        a_Edge* edge = create_aedge();
        edge->id = graph->nb_edges+1;

        int idx = 0;
        for (Node* cur = line_list->head; cur != NULL; cur = cur->next) {
            edge->vertices[idx] = cur->value;
            idx++;
        }
        edge->nb_vertices = idx;

        add_aedge(graph, edge);
    }

    fclose(file);
    if (line)
        free(line);

    //on init un tableau de correspondance pour que tout tienne sur un tableau
    //car sinon il y a des trous, beaucoup
    int corresp[MAX_VERTICES];
    memset(corresp, -1, sizeof(corresp));



    for (int i = 0; i < graph->nb_edges; i++) {
        a_Edge* edge = graph->edges[i];

        for (int j = 0; j < edge->nb_vertices; j++) {
            int value = edge->vertices[j];
            if (corresp[value-1] == -1) {
                corresp[value-1] = graph->nb_vertices;
            }
            int id = corresp[value-1];

            if (id >= graph->nb_vertices || graph->vertices[id] == NULL) {
                a_Vertex* vertex = create_avertex();
                vertex->id = value;  // Conserve l'ID original qui commence à 1
                add_avertex(graph, vertex);
            }
            //printf("%d %d %d \n", value, id, i);

            add_edge_to_vertex(graph, id, i);
        }
    }

    return graph;
}