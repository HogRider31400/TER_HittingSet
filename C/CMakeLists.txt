cmake_minimum_required(VERSION 3.30)
project(C C)

set(CMAKE_C_STANDARD 23)
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
add_executable(C main.c
        structures/List.c
        HGraph.c
        algorithms/naive.c
        graph_reader.c
        structures/List.h
        HGraph.h
        graph_reader.h
        algorithms/naive.h
        structures/Queue.c
        structures/Queue.h
        algorithms/berge.c
        algorithms/berge.h
        algorithms/DL.c
        algorithms/DL.h
        structures/bitmap.c
        structures/Bitmap.h
        algorithms/berge_bitmap.c
        algorithms/berge_bitmap.h
)
