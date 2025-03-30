H = {
    "vertices" : {
        1 : [0],
        2 : [0, 1],
        3 : [1]
    },
    "edges" : {
        0 : [1, 2],
        1 : [2, 3]
    }
}

import copy

d_H = [
    [2],
    [1, 3]
]

def add_vertices(H, eid, edge):
    for val in edge:
        if not val in H["vertices"]:
            H["vertices"][val] = []
        if not eid in H["vertices"][val]:
            H["vertices"][val].append(eid)

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
            if len(cur_edge) > 1:
                Hmx["edges"][edge] = copy.deepcopy(cur_edge)
                Hmx["edges"][edge].remove(x)
                add_vertices(Hmx, edge, Hmx["edges"][edge])
    
    return Hnx, Hmx

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

print(rec(H))
