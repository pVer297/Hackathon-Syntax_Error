cmake_minimum_required(VERSION 3.14)
project(tcp_srv_cpp)

set(CMAKE_CXX_STANDARD 14)
link_libraries(ws2_32 wsock32)

add_executable(tcp_srv_cpp main.cpp)