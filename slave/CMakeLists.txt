cmake_minimum_required(VERSION 2.6)
project(slave)

add_executable(slave main.cpp)

set(MPICH_C clang)
set(MPICH_CXX clang++)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_COMPILER "mpicc")
set(CMAKE_CXX_COMPILER "mpic++")
set(CMAKE_CXX_FLAGS "--std=c++14")
install(TARGETS slave RUNTIME DESTINATION bin)
