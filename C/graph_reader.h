//
// Created by Alex on 25/01/2025.
//

#ifndef GRAPH_READER_H
#define GRAPH_READER_H
#include "HGraph.h"

Graph* read_graph_from_file(char *filename);
iList* parse_line(char *line, int size);
size_t getline(char **lineptr, size_t *n, FILE *stream);
#endif //GRAPH_READER_H
