cmake_minimum_required(VERSION 3.14)
project(E_Commerce_Store C)

set(CMAKE_C_STANDARD 99)

#add_executable(E_Commerce_Store main.c hash_table/HashTable.c hash_table/HashTable.h server/store.c server/store.h util/exception_handling/Failure.c util/exception_handling/Failure.h util/client_utilities/ClientUtilities.c util/client_utilities/ClientUtilities.h clients/RegularClient.c clients/RegularClient.h util/server_utilities/ServerUtilities.c util/server_utilities/ServerUtilities.h)
add_executable(E_Commerce_Store_main main.c hash_table/HashTable.c hash_table/HashTable.h server/store.c server/store.h util/exception_handling/Failure.c util/exception_handling/Failure.h util/client_utilities/ClientUtilities.c util/client_utilities/ClientUtilities.h util/server_utilities/ServerUtilities.c util/server_utilities/ServerUtilities.h)
add_executable(E_Commerce_Store_client clients/RegularClient.c util/exception_handling/Failure.c util/exception_handling/Failure.h util/client_utilities/ClientUtilities.c util/client_utilities/ClientUtilities.h clients/RegularClient.c clients/RegularClient.h)
add_executable(E_Commerce_Store_server server/server.c hash_table/HashTable.c hash_table/HashTable.h util/exception_handling/Failure.c util/exception_handling/Failure.h util/server_utilities/ServerUtilities.c util/server_utilities/ServerUtilities.h server/server.c server/server.h)