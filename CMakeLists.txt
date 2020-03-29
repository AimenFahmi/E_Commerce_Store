cmake_minimum_required(VERSION 3.14)
project(E_Commerce_Store C)

set(CMAKE_C_STANDARD 99)

add_executable(E_Commerce_Store main.c hash_table/HashTable.c hash_table/HashTable.h sever/store.c sever/store.h util/Failure.c util/Failure.h clients/RegularClient.c clients/RegularClient.h)