cmake_minimum_required(VERSION 3.30)
project(C C)

set(CMAKE_C_STANDARD 23)
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/*.c")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/*.h")

list(FILTER SOURCES EXCLUDE REGEX ".*/CMakeFiles/.*")
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/test.c") # Celui-ci j'ai pas réussi à le faire compiler

find_package(Python3 REQUIRED COMPONENTS Development)

include_directories(${Python3_INCLUDE_DIRS})
link_directories(${Python3_LIBRARY_DIRS})

add_executable(C ${SOURCES} ${HEADERS})
target_link_libraries(C ${Python3_LIBRARIES})
