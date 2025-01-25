//
// Created by Alex on 25/01/2025.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
    int cur_nb_length = 0;

    for (int i = 0; i < size; i++) {
        if (line[i] == '\n') break;
        //Si c'est un espace on reset et on ajoute le nombre construit à la liste
        if (line[i] == ' ') {
            append(list,cur_nb);
            cur_nb = 0;
            cur_nb_length = 0;
        }
        else {
            //Ce n'est pas un espace donc à priori c'est un chiffre, on vérifie bien que c'est un chiffre
            //puis on l'ajoute au nombre construit
            int cur_digit = line[i] - '0';
            if (cur_digit > 9) {
                printf("wtf ??? %d \n" , cur_digit);
            }
            else {
                cur_nb += cur_digit * pow(10, cur_nb_length);
                cur_nb_length++;
            }
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
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    Graph* graph = create_graph();

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

    //On a toutes les hyper arêtes il faut reconstruire les sommets à partir de ça

    for (int i = 0; i < graph->nb_edges; i++) {
        Edge* edge = graph->edges[i];

        for (Node* cur = edge->vertices->head; cur != NULL; cur = cur->next) {
            //On vérifie si le sommet a déjà été init, si non on le fait
            if (graph->vertices[cur->value-1] == NULL) {
                graph->vertices[cur->value-1] = create_vertex();
                graph->vertices[cur->value-1]->id = cur->value;
                graph->nb_vertices++;
            }
            append(graph->vertices[cur->value-1]->edges, edge->id);
        }

    }
    //printf("La sommet 3 a :");
    //print_list(graph->vertices[2]->edges);
    //printf("\n");

    return graph;

}