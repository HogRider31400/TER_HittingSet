/***************************************************************
 * BMR_bitmap.c
 * Optimisation de l'algorithme BMR avec des opérations bitwise sur des bitmaps
 *
 * AUTEUR : Leandre GIAMMONA
 ***************************************************************/

 #include <stdio.h>
 #include <stdlib.h>
 #include "BMR_bitmap.h"
 #include "DL_BMR.h"
 #include "List.h"
 #include "Queue.h"
 #include "Bitmap.h"
 #include "../HGraph.h"
 
 extern iListList* globalTransversals;

 static int is_transversal_bitmap(Graph* graph, bitmap* bm);
 static int is_minimal_transversal_bitmap(Graph* graph, bitmap* bm);
 static void merge_transversals_bitmap(iListList* global, iListList* local, Graph* fullGraph);
 static bitmap* create_full_bitmap(int size);
 static void free_bitmap_if_needed(bitmap* bm);
 
 static int is_transversal_bitmap(Graph* graph, bitmap* bm) {
     for (int i = 0; i < graph->nb_edges; i++) {
         Edge* e = graph->edges[i];
         int covered = 0;
         for (Node* cur = e->vertices->head; cur != NULL; cur = cur->next) {
             if (get_bit(bm, cur->value)) {
                 covered = 1;
                 break;
             }
         }
         if (!covered) return 0;
     }
     return 1;
 }
 
 static int is_minimal_transversal_bitmap(Graph* graph, bitmap* bm) {
     for (int v = 0; v < bm->nb_bits; v++) {
         if (get_bit(bm, v)) {
             bitmap* reduced = copy_bitmap(bm);
             set_bit(reduced, v, 0); // En gros là on teste de retirer un sommet pour voir si c'est toujours valide 
             if (is_transversal_bitmap(graph, reduced)) {
                 free_bitmap(reduced);
                 return 0;  // Pas minimal si le transversal reste valide sans ce sommet
             }
             free_bitmap(reduced);
         }
     }
     return 1;
 }
 
 // Ca merge les transversaux qu'on a trouvé
 static void merge_transversals_bitmap(iListList* global, iListList* local, Graph* fullGraph) {
     for (NodeList* cur = local->head; cur != NULL; cur = cur->next) {
         if (!dl_bmr_contains_list(global, cur->value)) {
             append_list(global, deep_copy(cur->value));
         }
     }
 }
 
 // Crée un bitmap de taille 'size' avec tous les bits à 1
 static bitmap* create_full_bitmap(int size) {
     bitmap* bm = create_bitmap(size);
     for (int i = 0; i < size; i++) {
         set_bit(bm, i, 1);
     }
     return bm;
 }
 
 // Algorithme BMR avec bitmaps
 void BMR_bitmap_algorithm(Graph* graph, iList* Vpartition) {
     if (!graph || graph->nb_edges == 0) return;

     globalTransversals = create_list_list();
     bitmap* full_bitmap = create_full_bitmap(graph->nb_vertices);

     int max_combinations = 1 << graph->nb_vertices;
     for (int comb = 0; comb < max_combinations; comb++) {
         bitmap* bm = create_bitmap(graph->nb_vertices);
         for (int v = 0; v < graph->nb_vertices; v++) {
             set_bit(bm, v, (comb >> v) & 1);
         }
 
         if (is_transversal_bitmap(graph, bm) && is_minimal_transversal_bitmap(graph, bm)) {
             iList* transversal = create_list();
             for (int v = 0; v < graph->nb_vertices; v++) {
                 if (get_bit(bm, v)) append(transversal, v + 1);
             }
             append_list(globalTransversals, transversal);
         }
         free_bitmap(bm);
     }
     free_bitmap(full_bitmap);
 }

 void launch_bmr_bitmap(Graph* graph) {
     globalTransversals = create_list_list();
     iList* Vpartition = create_list(); 
     BMR_bitmap_algorithm(graph, Vpartition);  
     free_list(Vpartition);  
 

     printf("Transversaux finaux avec bitmaps:\n");
     print_list_list(globalTransversals);
     free_list_list(globalTransversals);
 }
 