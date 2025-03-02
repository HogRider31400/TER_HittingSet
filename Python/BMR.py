from graph_reader import graph_reader

vertices, edges = graph_reader("SDFP9.txt")

def order_vertices(edge_set) :
    """
    Fonction qui retourne la liste des sommets dans l'ordre des ocurrences qu'elles ont dans edges.
    """
    graph_dic = {}
    for x in vertices :
        graph_dic[x] = 0
    for edge in edge_set :
        for element in edge :
            graph_dic[element] += 1
    sorted_vertices = sorted(graph_dic.items(), key=lambda x:x[1], reverse=False)
    return [el[0] for el in sorted_vertices]

def BMR(edge_set, vertices) :
    """
    Fonction qui applique l'algorithme BMR sur un hypergraphe.
    """
    sorted_vertices = order_vertices(edge_set)
    vertices = vertices
    for i in range(len(sorted_vertices)) :
        E_partition = []
        vertices.remove(sorted_vertices[i])
        for edge in edge_set :
            if sorted_vertices[i] not in edge :
                E_partition = min()