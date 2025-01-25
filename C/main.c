#include <stdio.h>
#include "List.c"
#include "HGraph.c"

int main(void) {
    printf("Hello, World!\n");
    iList* list;
    list = create_list();

    append(list, 10);
    append(list, 20);
    append(list, 30);

    print_list(list);
    printf("\n");

    Graph* graph = create_graph();

    Vertex* n1 = create_vertex();
    n1->id = 1;
    append(n1->edges,1);
    append(n1->edges,3);

    Vertex* n2 = create_vertex();
    n2->id = 2;
    append(n2->edges,1);
    append(n2->edges,4);

    Vertex* n3 = create_vertex();
    n3->id = 3;
    append(n3->edges,2);
    append(n3->edges,4);

    Vertex* n4 = create_vertex();
    n4->id = 4;
    append(n4->edges,2);
    append(n4->edges,3);

    add_vertex(graph,n1);
    add_vertex(graph,n2);
    add_vertex(graph,n3);
    add_vertex(graph,n4);

    Edge* e1 = create_edge();
    e1->id = 1;
    append(e1->vertices, 1);
    append(e1->vertices, 2);

    Edge* e2 = create_edge();
    e2->id = 2;
    append(e2->vertices, 3);
    append(e2->vertices, 4);

    Edge* e3 = create_edge();
    e3->id = 3;
    append(e3->vertices, 1);
    append(e3->vertices, 4);

    Edge* e4 = create_edge();
    e4->id = 4;
    append(e4->vertices, 2);
    append(e4->vertices, 3);

    add_edge(graph,e1);
    add_edge(graph,e2);
    add_edge(graph,e3);
    add_edge(graph,e4);

    for (int i = 0; i < graph->nb_vertices;i++) {
        printf("Noeud %d : ", graph->vertices[i]->id);
        print_list(graph->vertices[i]->edges);
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < graph->nb_edges; i++) {
        printf("Arete %d : ", graph->edges[i]->id);
        print_list(graph->edges[i]->vertices);
        printf("\n");
    }


    return 0;
}