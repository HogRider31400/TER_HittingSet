H = {
     "vertices" : {
         1 : [0],
         2 : [0, 2],
         3 : [0, 1],
         4 : [1, 2],
         5 : [1]
     },
     "edges" : {
         0 : [1, 2, 3],
         1 : [3, 4, 5],
         2 : [2, 4]
     }
 }

import copy

d_H = {
    "vertices" : {
        1 : [2],
        2 : [0,3,4],
        3 : [0, 1],
        4 : [1,2,3],
        5 : [4]
    },
    "edges" : {
        0 : [2, 3],
        1 : [3, 4],
        2 : [1, 4],
        3 : [2, 4],
        4 : [2, 5]
    }
}

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



def rec(H):
    print(H)
    if len(H["edges"].keys()) == 0:
        return [[]]
    
    if len(H["vertices"].keys()) == 0:
        return [None]
    
    x = list(H["vertices"].keys())[0]
    Hnx, Hmx = split(H, x)
    print("Le split sur", x, ":", Hnx, Hmx)
    res = rec(Hnx)
    print("res 0:", res)
    if(res == [None]):
        res = []
    for elem in res:
        elem.append(x)
    print("res 1 :", res)
    res2 = rec(Hmx)
    if(res2 == [None]):
        res2 = []
    print("res 2 :", res)
    print("le min Tr:", minTr(res + res2))
    return minTr(res + res2)

#Teste si H et G sont duaux, donc si G représente l'hypergraphe des transversaux minimaux de H
def FK_A(H,G):

    #cas terminaux
    #if len(H["edges"].keys()) == 0: or len(G["edges"].keys()) == 0:
    #    return False
    if len(H["edges"].keys()) == 0:
        print("On est sur la fin :")
        print("H :", H)
        print("G :", G)
        return len(G["edges"].keys()) == 1 and list(G["edges"].values())[0] == []
        
    if len(G["edges"].keys()) == 0:
        print("On est sur la fin :")
        print("H :", H)
        print("G :", G)
        return len(H["edges"].keys()) == 1 and list(H["edges"].values())[0] == []
        
    
    if len(H["edges"].keys()) * len(G["edges"].keys()) == 1:
        tH = list(H["edges"].values())[0]
        tG = list(G["edges"].values())[0]
        tH.sort()
        tG.sort()
        print("Cas terminal tH tG :")
        print(tH,tG, tH == tG)
        return tH == tG
    print("H :", H)
    print("G :", G)
    if len(H["vertices"]) > 0:
        x = list(H["vertices"].keys())[0]
    else:
        x = list(G["vertices"].keys())[0]
    Hnx, Hmx = split(H, x)
    Gnx, Gmx = split(G, x)

    print(f"Variable choisie: {x}")
    print("Hnx:", Hnx)
    print("Hmx:", Hmx)
    print("Gnx:", Gnx)
    print("Gmx:", Gmx)

    H_union = hyper_union(Hnx, Hmx)
    G_union = hyper_union(Gnx, Gmx)

    print("Union Hnx ∪ Hmx :", H_union)
    print("Union Gnx ∪ Gmx :", G_union)
    #On regarde les transversaux sans x de base dans H et on enlève complètement x de G
    r1 = FK_A(Hnx, Gmx)

    #Maintenant on regarde pour G sans x de base, et on enlève complètement x de H
    r2 = FK_A(Gnx, Hmx)

    return r1 and r2

print(FK_A(H, d_H))