import copy

#Pour la notation on aura ici :
#Un dico de sommet -> liste d'arêtes qui couvrent le sommet
#Un dico d'arêtes -> liste de sommets couvertent par le sommet

vertices = {
    1 : [1,3],
    2 : [1,4],
    3 : [2,4],
    4 : [2,3]
}

edges = {
    1 : [1,2],
    2 : [3,4],
    3 : [1,4],
    4 : [2,3]
}

def covers(cur_vertices):
    for vertice in vertices:
        if not vertice in cur_vertices: return False
    return True


def enum_covers(cur_covered_vertices=list(vertices.keys()), cur_used_edges=list(edges.keys())):
    for edge in cur_used_edges:
        new_edges = copy.copy(cur_used_edges)
        new_edges.remove(edge)
        new_vertices = []
        for edge in new_edges:
            for vertice in edges[edge]:
                new_vertices.append(vertice)
        if covers(new_vertices):
            print(new_edges)
            enum_covers(new_vertices, new_edges)  

enum_covers()