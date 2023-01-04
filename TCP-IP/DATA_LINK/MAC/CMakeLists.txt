cmake_minimum_required(VERSION 3.10)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(MAC_testing)

# add the executable
add_executable(test MAC.c test_MAC.c ../../utils.c)