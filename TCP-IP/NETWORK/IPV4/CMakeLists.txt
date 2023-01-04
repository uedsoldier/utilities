cmake_minimum_required(VERSION 3.10)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(IPV4_testing)

# add the executable
add_executable(test IPv4.c test_IPv4.c ../../utils.c)