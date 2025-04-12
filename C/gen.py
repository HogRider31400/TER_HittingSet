
import random
import os
def gen_write(name, size, density):
    v = [x for x in range(1,size+1)]

    nb_e = random.randint(1, size//2)
    seen = set()
    cur = 0
    with open(name,"w") as f:
        while cur < nb_e or len(seen) != len(v):
            e = tuple(sorted(random.sample(v, 
                            max(1, min(size-1, abs(int(random.gauss(mu=density, sigma=4))))))))
            cur += 1
            for elem in e:
                seen.add(elem)
            f.write(" ".join(str(x) for x in e) + "\n")
#On va de 2 à 100 de densité ! (incrément de 10)

density = 2
while density < 100:
    
    newpath = "./data/random/"+str(density)+"/"
    if not os.path.exists(newpath):
        os.makedirs(newpath)

    for i in range(18):
        size = random.randint(density, 120)
        name = "./data/random/"+str(density)+"/random_"+str(size)+".dat"

        gen_write(name,size,density)
    
    density += 10


