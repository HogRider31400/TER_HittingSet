cmake_minimum_required(VERSION 3.30)
project(C C)

set(CMAKE_C_STANDARD 23)

add_executable(C main.c
        List.c
        HGraph.c
        naive.c)
