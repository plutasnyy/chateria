cmake_minimum_required(VERSION 3.12)
project(backend)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

add_executable(backend main.cpp ThreadData.cpp ThreadData.h GlobalData.cpp GlobalData.h)