import sys
import time

print_f = sys.stdout.write

l_t = {}

for cur in range(20, 47, 2):

    t0 = time.time()
    n = cur

    hg = [(i,i+1) for i in range(1, n+1, 2)]
    #print(hg)
    Trs = [[]]
    
    for elem in hg:
        a,b = elem

        n_Trs = []

        for elem_2 in Trs:
            n_Trs.append(elem_2 + [a])
            n_Trs.append(elem_2 + [b])
        
        Trs = n_Trs

    for elem in Trs:
        print_f(' '.join(str(x) for x in elem) + "\n")

    t1 = time.time()
    l_t[cur] = t1-t0

print(l_t)