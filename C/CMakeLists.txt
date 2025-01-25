cmake_minimum_required(VERSION 3.30)
project(C C)

set(CMAKE_C_STANDARD 23)
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
add_executable(C main.c
        List.c
        HGraph.c
        naive.c
        graph_reader.c
        List.h
        HGraph.h
        graph_reader.h
        naive.h)
