cmake_minimum_required(VERSION 3.10)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(IPV6_testing)

# add the executable
add_executable(test IPv6.c test_IPv6.c ../../utils.c)