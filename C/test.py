import subprocess
import os
import signal
import psutil
import threading
import time
import re

oom_list = []


algorithms = [
    #"naive_recursive",
    #"naive_iterative",
    #"naive_iterative_array",
    #"berge",
    #"#python_it",
    #"naive_iterative_array_2",
    "dong_li",
    "DL_opti",
    #"DL_singl", # Celui-là est faux
    "DL_Trgf",
    "DL_Trgn",
    "DL_fopti",
    "DL_foptiv",
    #"berge_bitmap"
]
TEST_TYPE = "/preprocess/"
executable_c = ["build/C.exe"]
executable_python = ["python" ,"C/from_empty.py"]
test_files = [f for f in os.listdir("C/data" + TEST_TYPE) if f.endswith('.dat')]
check_files = [f for f in os.listdir("C/data/tests/inputs") if f.endswith('.txt')]

def parse_hypergraph(file_path):
    #Faire très attention à le parse de la même manière qu'en C, pas très dur
    
    hypergraph = {
        "edges" : [],
        "vertices" : {}
    }

    with open(file_path) as f:
        cur = 0
        for line in f:
            l_slices = re.split(r'[|#]', line)
            line = l_slices[0]
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

def monitor_memory(process, limit_gb=4):
    limit_bytes = limit_gb * 1024 * 1024 * 1024
    p = psutil.Process(process.pid)
    
    while process.poll() is None:
        try:
            if p.memory_info().rss > limit_bytes:
                oom_list.append(p.memory_info().rss)
                process.send_signal(signal.SIGTERM)
                break
            time.sleep(0.1)
        except (psutil.NoSuchProcess, psutil.AccessDenied):
            break

def get_covers_and_time(executable, algorithm, test_path, output=False):
    hypergraph = parse_hypergraph(test_path)
    #print(hypergraph)
    try:
        process = subprocess.Popen([*executable, algorithm, test_path],
                             stdout=subprocess.PIPE, 
                             stderr=subprocess.PIPE)

        monitor_thread = threading.Thread(target=monitor_memory, args=(process, 4))
        monitor_thread.daemon = True
        monitor_thread.start()
    
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

#get_covers_and_time(executable_c, "naive_iterative", "C:/Users/Alex/Desktop/TER_HittingSet/C/data/tests/inputs/simple.txt")
def exec_test(algorithm, test_path, executable = executable_c):
    covers,time, hypergraph = get_covers_and_time(executable, algorithm, test_path)
    if time == -1: return covers
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

def exec_check(algorithm, check_file, executable = executable_c):
    check_inp = "C/data/tests/inputs/" + check_file
    check_out = "C/data/tests/outputs/" + check_file

    covers,time,_ = get_covers_and_time(executable, algorithm, check_inp)

    excepted_output = []

    for line in open(check_out):
        #print(line)
        excepted_output.append(list(map(int, line.split())))
        excepted_output[-1].sort()
    
    excepted_output.sort()
    #print(covers)
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

    
    if algorithm[0] == "#":
        executable = executable_python 
    else:
        executable = executable_c

    test_ct = 0
    for check_file in check_files:
        result = exec_check(algorithm, check_file, executable)
        ##if result["is_correct"]:
        #    print(f"{algorithm}: {check_file} - Correct")
        if result["is_correct"]:
            test_ct += 1
        else:
            print(f"{algorithm}: {check_file} - Incorrect")
            print(f"Expected: {result['excepted']}")
            print(f"Got: {result['got']}")
    if test_ct == len(check_files): 
            print(f"{algorithm}: Correct")
print()
print("Tests de performance sur le dataset " + TEST_TYPE.replace("/","\""))
print()
#test_files.sort(key = lambda x: int(x.replace("TH","").replace(".dat","")))
ml = max([len(s) for s in algorithms])
header = " " * max([len(s) for s in algorithms])

for test_file in test_files:
    
    header += " | " + test_file

print(header)


#Le temps
results = {}
for algorithm in algorithms:

    if algorithm[0] == "#":
        executable = executable_python 
    else:
        executable = executable_c

    cur_results = {}
    has_failed = False
    for test_file in test_files:
        if has_failed:
            cur_results[test_file] = {"time" : -1}
        else:
            cur_results[test_file] = exec_test(algorithm,"C/data" + TEST_TYPE + test_file,executable)
            if(cur_results[test_file]["time"] == -1):
                has_failed = True

    results[algorithm] = cur_results

    line = algorithm + " " * (ml - len(algorithm))

    for cur_t in test_files:
        line += " | "
        #print(cur_results)
        c_time = str(cur_results[cur_t]["time"])[:7]

        line += " " * (len(cur_t) - len(str(c_time)))
        line += str(c_time)
    print(line)

print(oom_list)




#print(exec_test(algorithms[0], test_files[0]))

