cmake_minimum_required(VERSION 3.14)
project(E_Commerce_Store C)

set(CMAKE_C_STANDARD 99)

add_executable(E_Commerce_Store_main main.c hash_table/HashTable.c hash_table/HashTable.h server/store.c server/store.h util/exception_handling/Failure.c util/exception_handling/Failure.h util/client_utilities/ClientUtilities.c util/client_utilities/ClientUtilities.h util/server_utilities/ServerUtilities.c util/server_utilities/ServerUtilities.h util/random_utilities/RandomUtilities.c util/random_utilities/RandomUtilities.h)
add_executable(E_Commerce_Store_Shop clients/Shop.c util/exception_handling/Failure.c util/exception_handling/Failure.h util/client_utilities/ClientUtilities.c util/client_utilities/ClientUtilities.h clients/Shop.c clients/Shop.h util/random_utilities/RandomUtilities.c util/random_utilities/RandomUtilities.h)
add_executable(E_Commerce_Store_Customer clients/Customer.c util/exception_handling/Failure.c util/exception_handling/Failure.h util/client_utilities/ClientUtilities.c util/client_utilities/ClientUtilities.h clients/Customer.c clients/Customer.h util/random_utilities/RandomUtilities.c util/random_utilities/RandomUtilities.h)
add_executable(E_Commerce_Store_server server/server.c hash_table/HashTable.c hash_table/HashTable.h util/exception_handling/Failure.c util/exception_handling/Failure.h util/server_utilities/ServerUtilities.c util/server_utilities/ServerUtilities.h server/server.c server/server.h server/store.h server/store.c util/random_utilities/RandomUtilities.c util/random_utilities/RandomUtilities.h)