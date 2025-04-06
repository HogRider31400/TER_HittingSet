import sys
import time

print_f = sys.stdout.write

l_t = {}

for cur in range(20, 47, 2):

    t0 = time.time()
    n = cur

    hg = [(i,i+1) for i in range(1, n+1, 2)]
    #print(hg)

    def gen(i):
        if i == n/2-1:
            return [[hg[i][0],], [hg[i][1],]]
        
        res = gen(i+1)
        a,b = hg[i]
        new = []
        for elem in res:
            new.append([a] + elem)
            new.append([b] + elem)
        return new

    for elem in gen(0):
        print_f(' '.join(str(x) for x in elem) + "\n")

    t1 = time.time()
    l_t[cur] = t1-t0

print(l_t)