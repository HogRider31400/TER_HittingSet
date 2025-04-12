
#Pour la notation on aura ici :
#Un dico de sommet -> liste d'arêtes qui couvrent le sommet
#Un dico d'arêtes -> liste de sommets couvertent par le sommet

import sys
import time

import transversal_enumeration as t
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

def enum_covers(edges):
    Tr = [[v] for v in edges[0]]

    for i in range(1, len(edges)):
        c_Tr = [[v] for v in edges[i]]

        n_Tr = [list(set(x + y)) for x in Tr for y in c_Tr]
        n_Tr.sort(key = len)
        f_Tr = []

        for elem in n_Tr:
            if not has_subset(f_Tr, elem):
                f_Tr.append(elem)
        Tr = f_Tr
    
    return Tr

def get_comp(start):
    comp = []
    vu = set()
    e_vu = set()
    queue = [start]

    while len(queue) != 0:
        cur = queue.pop(-1)
        vu.add(cur)
        
        for edge in h["vertices"][cur]:
            if edge in e_vu:
                continue
            
            e_vu.add(edge)
            comp.append(edge)
            for vertice in h["edges"][edge]:
                if vertice in vu:
                    continue
                vu.add(vertice)
                queue.append(vertice)
    
    return comp, vu

def get_comps():
    comps = []
    vu = set()

    for vertice in h["vertices"]:
        if vertice in vu:
            continue
        n_comp,n_vu = get_comp(vertice)

        vu = vu | n_vu
        comps.append(n_comp)
    
    return comps

t1 = time.time()
Trs = t.dong_li(edges)
for tr in Trs:
    print(' '.join(str(x) for x in tr) + "\n")
t2 = time.time()

print("Time\n")
print(str(t2-t1))
sys.stdout.flush()