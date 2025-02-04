from graph_reader import graph_reader

vertices = graph_reader("example.txt")[0]
edges = graph_reader("example.txt")[1]

def order_vertices(edge_set) :
    """
    Fonction qui retourne la liste des sommets dans l'ordre des ocurrences qu'elles ont dans edges.
    """
    vertices_temp = [0 for _ in range(len(vertices))]
    for edge in edge_set:
        for vertice in edge:
            vertices_temp[vertice-1] += 1
    return