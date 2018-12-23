cmake_minimum_required(VERSION 3.12)
project(backend)

include_directories(/usr/include/openssl/)
link_libraries(crypto)

set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -std=c++17 -pthread -Wall -lcrypto -lssl")
add_executable(backend main.cpp json.hpp ThreadData.cpp ThreadData.h GlobalData.cpp GlobalData.h Base64.cpp Base64.h WebsocketContent.cpp WebsocketContent.h)