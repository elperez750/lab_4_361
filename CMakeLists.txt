cmake_minimum_required(VERSION 4.3)
project(lab_4_cs361 C)

set(CMAKE_C_STANDARD 17)

add_executable(lab_4_cs361 main.c
        crud_functions.c
        crud_functions.h
        structures.h
        structures.h
        helper_functions.c
        helper_functions.h)
