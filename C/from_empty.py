
#Pour la notation on aura ici :
#Un dico de sommet -> liste d'arêtes qui couvrent le sommet
#Un dico d'arêtes -> liste de sommets couvertent par le sommet

import sys
import time

print = sys.stdout.write

def parse_hypergraph(file_path):
    #Faire très attention à le parse de la même manière qu'en C, pas très dur
    
    hypergraph = {
        "edges" : [],
        "vertices" : {}
    }

    with open(file_path) as f:
        cur = 0
        for line in f:
            line = line.replace("\n","")
            #print(line)
            cur_vertices = list(map(int, line.split()))
            hypergraph["edges"].append(cur_vertices)

            for vertice in cur_vertices:
                if vertice in hypergraph["vertices"]:
                    hypergraph["vertices"][vertice].add(cur)
                else:
                    hypergraph["vertices"][vertice] = set([cur])
            cur += 1

    return hypergraph

if len(sys.argv) < 3: sys.exit()

alg = sys.argv[1]
file = sys.argv[2]

h = parse_hypergraph(file)

sols = set()
edges = h["edges"]
vertices = h["vertices"]

def covers(cur_edges):
    return len(edges) == len(cur_edges)

def is_subset(l, sl):
    for elem in sl:
        if elem not in l:
            return False
    return True

def has_subset(bl, l):
    for sub_l in bl:
        if is_subset(l, sub_l):
            return True
    return False

def enum_covers():
    queue = [([],[])]

    while len(queue) > 0:
        cur = queue.pop(0)
        cur_covered_edges = cur[0]
        cur_used_vertices = cur[1]
        if(covers(cur_covered_edges)):
            if not has_subset(sols, cur_used_vertices):
                sols.add(tuple(cur_used_vertices))
                print(' '.join(str(x) for x in cur_used_vertices) + "\n")
            continue
        deb = 1
        if len(cur_used_vertices) > 0:
            deb = cur_used_vertices[-1]
        for i in range(deb, len(vertices)+1):
            if i in cur_used_vertices: continue

            new_edges = set(cur_covered_edges)

            for edge in vertices[i]:
                new_edges.add(edge)
            
            queue.append((list(new_edges), cur_used_vertices + [i]))

t1 = time.time()
enum_covers()
t2 = time.time()

print("Time\n")
print(str(t2-t1))