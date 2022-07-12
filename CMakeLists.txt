cmake_minimum_required(VERSION 3.16.3)
project(testing C)

set(CMAKE_C_STANDARD 11)

add_executable(testing main.c functions.c functions.h internal.c linkedlist.c stack.c external.c linenoise/linenoise.c)
