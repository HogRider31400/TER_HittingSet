
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


def enum_covers(cur_covered_vertices=[], cur_used_vertices=[]):
    if covers(cur_covered_vertices):
        print(cur_used_vertices)
    
    for vertice in vertices:
        if vertice in cur_used_vertices: continue
        taken_vertices = cur_covered_vertices
        for edge in vertices[vertice]:
            taken_vertices = list(set(taken_vertices + edges[edge]))
        enum_covers(taken_vertices, cur_used_vertices + [vertice])

enum_covers()