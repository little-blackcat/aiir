cmake_minimum_required(VERSION 2.6)
project(slave)

add_executable(slave IbcastExtensionForBoostMPI.hpp main.cpp Worker.cpp)

set(MPICH_C clang)
set(MPICH_CXX clang++)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_COMPILER "mpicc")
set(CMAKE_CXX_COMPILER "mpic++")
set(CMAKE_CXX_FLAGS "--std=c++14 -lboost_mpi -lboost_serialization")
install(TARGETS slave RUNTIME DESTINATION bin)
