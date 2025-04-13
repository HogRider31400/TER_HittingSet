import subprocess
import os
import signal
import psutil
import threading
import time
import sys
import copy
def parse_hypergraph(file_path):
    #Faire très attention à le parse de la même manière qu'en C, pas très dur
    
    hypergraph = {
        "edges" : {},
        "vertices" : {}
    }

    with open(file_path) as f:
        cur = 0
        nl = 0
        for line in f:
            line = line.replace("\n","")
            #print(line)
            cur_vertices = list(map(int, line.split()))
            hypergraph["edges"][cur] = cur_vertices

            for vertice in cur_vertices:
                if vertice in hypergraph["vertices"]:
                    hypergraph["vertices"][vertice].add(cur)
                else:
                    hypergraph["vertices"][vertice] = set([cur])
            cur += 1

    return hypergraph


def get_covers_and_time(executable, algorithm, test_path, output=False):
    hypergraph = parse_hypergraph(test_path)
    #print(hypergraph)
    try:
        process = subprocess.Popen([*executable, algorithm, os.path.abspath(test_path).replace("\\","/")],
                             stdout=subprocess.PIPE, 
                             stderr=subprocess.PIPE)

        test_output = process.communicate(timeout=120)
    except subprocess.TimeoutExpired:
        #print("alo")
        process.kill()
        return {"time": -1}, -1, None
    #print(test_output, executable, algorithm, os.path.abspath(test_path).replace("\\","/"))
    if len(test_output[0]) == 0: return {"time": "OOM"}, -1, None
    test_output = test_output[0].decode('utf-8').split("\n")
    if len(test_output) == 1: return {"time": "OOM"}, -1, None
    covers = []
    time = -1
    process.kill()
    #print(executable, algorithm, test_path,test_output)
    expect_time = False
    for line in test_output:
        if line.startswith("Time"):
            expect_time = True
        elif expect_time:
            time = float(line)
        else:
            covers.append(list(map(int, line.split())))
            covers[-1].sort()
    return covers, time, hypergraph



def add_vertices(H, eid, edge):
    for val in edge:
        if not val in H["vertices"]:
            H["vertices"][val] = []
        if not eid in H["vertices"][val]:
            H["vertices"][val].append(eid)

def merge_in(l1,l2):
    #A faire efficacement, là c en N^2 mdr

    for elem in l2:
        if not elem in l1:
            l1.append(elem)

def split(H, x):
    #H ou y'a pas x de base
    Hnx = {
        "vertices" : {},
        "edges" : {}
    }
    #H où on a enlevé x
    Hmx = {
        "vertices" : {},
        "edges" : {}
    }

    for edge in H["edges"]:
        cur_edge = H["edges"][edge]
        if not x in cur_edge:
            Hnx["edges"][edge] = copy.deepcopy(cur_edge)
            #Hmx["edges"][edge] = copy.deepcopy(cur_edge)
            add_vertices(Hnx, edge, cur_edge)
            #add_vertices(Hmx, edge, cur_edge)
        else:
            if len(cur_edge) >= 1:
                Hmx["edges"][edge] = copy.deepcopy(cur_edge)
                Hmx["edges"][edge].remove(x)
                add_vertices(Hmx, edge, Hmx["edges"][edge])
    
    return Hnx, Hmx

def hyper_union(H1,H2):
    Hu = {
        "vertices" : {},
        "edges" : {}
    }

    edges = list(copy.deepcopy(H1["edges"]).values()) + list(copy.deepcopy(H2["edges"]).values())
    edges.sort(key=len)
    
    for edge in edges:
        e_id = len(Hu["edges"])

        if not has_subset(Hu["edges"].values(), edge):
            Hu["edges"][e_id] = edge
            add_vertices(Hu, e_id, edge)

    return Hu


#On regarde si B C A
def is_subset(A,B):
    for elem in B:
        if not elem in A:
            return False
    return True

#On regarde si il existe a € A tq B C a
def has_subset(A, B):
    for elem in A:
        if is_subset(B, elem):
            return True
    return False

def minTr(Tr):
    Tr.sort(key=len)
    mTr = []
    for elem in Tr:
        if len(elem) == 0: continue
        if not has_subset(mTr, elem):
            mTr.append(elem)
    return mTr

DEBUG = False

#Teste si H et G sont duaux, donc si G représente l'hypergraphe des transversaux minimaux de H
def FK_A(H,G):

    #cas terminaux
    #if len(H["edges"].keys()) == 0: or len(G["edges"].keys()) == 0:
    #    return False
    if len(H["edges"].keys()) == 0:
        if DEBUG:
            print("On est sur la fin :")
            print("H :", H)
            print("G :", G)
        return len(G["edges"].keys()) == 1 and list(G["edges"].values())[0] == []
        
    if len(G["edges"].keys()) == 0:
        if DEBUG:
            print("On est sur la fin :")
            print("H :", H)
            print("G :", G)
        return len(H["edges"].keys()) == 1 and list(H["edges"].values())[0] == []
        
    
    if len(H["edges"].keys()) * len(G["edges"].keys()) == 1:
        tH = list(H["edges"].values())[0]
        tG = list(G["edges"].values())[0]
        tH.sort()
        tG.sort()
        if DEBUG:
            print("Cas terminal tH tG :")
            print(tH,tG, tH == tG)
        return tH == tG
    if DEBUG:
        print("H :", H)
        print("G :", G)
    if len(H["vertices"]) > 0:
        x = list(H["vertices"].keys())[0]
    else:
        x = list(G["vertices"].keys())[0]
    Hnx, Hmx = split(H, x)
    Gnx, Gmx = split(G, x)
    if DEBUG:
        print(f"Variable choisie: {x}")
        print("Hnx:", Hnx)
        print("Hmx:", Hmx)
        print("Gnx:", Gnx)
        print("Gmx:", Gmx)

    H_union = hyper_union(Hnx, Hmx)
    G_union = hyper_union(Gnx, Gmx)
    if DEBUG:
        print("Union Hnx ∪ Hmx :", H_union)
        print("Union Gnx ∪ Gmx :", G_union)
    #On regarde les transversaux sans x de base dans H et on enlève complètement x de G
    r1 = FK_A(Hnx, hyper_union(Gmx,Gnx))

    #Maintenant on regarde pour G sans x de base, et on enlève complètement x de H
    r2 = FK_A(Gnx, hyper_union(Hmx,Hnx))

    return r1 and r2

if len(sys.argv) < 2: sys.exit()
executable_c = ["./cmake-build-debug-1/C.exe"]
executable_python = ["python" ,"./from_empty.py"]
alg = sys.argv[1]

executable = executable_c
if alg.startswith("#"):
    executable = executable_python

test_files = [f for f in os.listdir("./data/random/") if f.endswith('.dat')]
#print(alg, test_files)
#H = parse_hypergraph("./data/tests/inputs/weird_T.txt")
#d_H = parse_hypergraph("./data/tests/outputs/weird_T.txt")
fine = True
for test_file in test_files:
    H = parse_hypergraph("./data/random/" + test_file)
    c, _, _ = get_covers_and_time(executable, alg, "./data/random/" + test_file)
    d_H = {
        "edges" : {},
        "vertices" : {}
    }

    cur = 0
    for cur_vertices in c:
        d_H["edges"][cur] = cur_vertices

        for vertice in cur_vertices:
            if vertice in d_H["vertices"]:
                d_H["vertices"][vertice].add(cur)
            else:
                d_H["vertices"][vertice] = set([cur])
        cur += 1
    res = FK_A(H,d_H)
    if res == False:
        print("Erreur de dualisation entre : ")
        print(H)
        print("Et : ")
        print(d_H)
        fine = False
    #print(FK_A(H,d_H))
if fine:
    print("Tout est passé !")
    print("Tests de correction automatiques finis !")