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


def enum_covers(cur_covered_vertices=list(vertices.keys()), cur_used_vertices=list(vertices.keys())):
    for vertice in cur_used_vertices:
        new_used_vertices = copy.copy(cur_used_vertices)
        new_used_vertices.remove(vertice)
        new_vertices = []
        for cur_used_vertice in new_used_vertices:
            for edge in vertices[cur_used_vertice]:
                new_vertices = list(set(new_vertices + edges[edge]))
        if covers(new_vertices):
            print(new_used_vertices)
            enum_covers(new_vertices, new_used_vertices)  
#Manque la couverture qui prend toutes les vertices, mais pg
enum_covers()