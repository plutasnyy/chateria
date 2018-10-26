cmake_minimum_required(VERSION 3.12)
project(testClient)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

add_executable(testClient main.cpp)