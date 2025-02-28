import subprocess
import os

algorithms = [
    "naive_recursive",
    "naive_iterative",
    "berge"
]

executable = "./cmake-build-debug/C.exe"

test_files = [f for f in os.listdir("./data") if f.endswith('.txt')]
check_files = [f for f in os.listdir("./data/tests/inputs") if f.endswith('.txt')]

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

def do_covers(hypergraph, cover):

    covered_vertices = set()

    for vertice in cover:
        for edge in hypergraph["vertices"][vertice]:
            for linked_vertice in hypergraph["edges"][edge]:
                covered_vertices.add(linked_vertice)
    
    if len(covered_vertices) == len(hypergraph["vertices"]):
        return True
    return False


def exec_test(algorithm, test_path):

    hypergraph = parse_hypergraph(test_path)
    #print(hypergraph)
    try:
        test_output = subprocess.run([executable, algorithm, test_path], stdout=subprocess.PIPE, timeout=60).stdout.decode('utf-8')
    except subprocess.TimeoutExpired:
        #print("Temps expiré")
        return {"time": -1}
    test_output = test_output.split("\n")

    covers = []
    time = -1

    expect_time = False
    for line in test_output:
        if line.startswith("Time"):
            expect_time = True
        elif expect_time:
            time = float(line)
        else:
            covers.append(list(map(int, line.split())))
    
    nb_covered = 0

    for cover in covers:
        if do_covers(hypergraph,cover):
            nb_covered += 1
        #print(do_covers(hypergraph,cover))

    result = {
        "nb_good_covers" : nb_covered,
        "nb_covers" : len(covers),
        "time" : time
    }

     #print(algorithm, covers)

    return result

def exec_check(algorithm, check_file):
    check_inp = "./data/tests/inputs/" + check_file
    check_out = "./data/tests/outputs/" + check_file
    hypergraph = parse_hypergraph(check_inp)

    try:
        test_output = subprocess.run([executable, algorithm, check_inp], stdout=subprocess.PIPE, timeout=60).stdout.decode('utf-8')
    except subprocess.TimeoutExpired:
        #print("Temps expiré")
        return {"time": -1}
    test_output = test_output.split("\n")

    covers = []
    time = -1

    expect_time = False
    for line in test_output:
        if line.startswith("Time"):
            expect_time = True
        elif expect_time:
            time = float(line)
        else:
            covers.append(list(map(int, line.split())))

    excepted_output = []

    for line in open(check_out):
        excepted_output.append(list(map(int, line.split())))
    
    excepted_output.sort()
    covers.sort()

    if covers == excepted_output:
        return {"is_correct": True}
    else:
        return {"is_correct": False, "excepted": excepted_output, "got": covers}

print()
print("Tests de correction")
print()

#La correction
for algorithm in algorithms:

    for check_file in check_files:
        result = exec_check(algorithm, check_file)
        if result["is_correct"]:
            print(f"{algorithm}: {check_file} - Correct")
        else:
            print(f"{algorithm}: {check_file} - Incorrect")
            print(f"Expected: {result['excepted']}")
            print(f"Got: {result['got']}")
print()
print("Tests de performance")
print()
ml = max([len(s) for s in algorithms])
header = " " * max([len(s) for s in algorithms])

for test_file in test_files:
    
    header += " | " + test_file

print(header)


#Le temps
results = {}
for algorithm in algorithms:

    cur_results = {}

    for test_file in test_files:
        cur_results[test_file] = exec_test(algorithm,"./data/" + test_file)

    results[algorithm] = cur_results

    line = algorithm + " " * (ml - len(algorithm))

    for cur_t in test_files:
        line += " | "
        c_time = cur_results[cur_t]["time"]

        line += " " * (len(cur_t) - len(str(c_time)))
        line += str(c_time)
    print(line)

print(results)




#print(exec_test(algorithms[0], test_files[0]))

