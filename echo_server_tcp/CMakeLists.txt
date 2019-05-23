cmake_minimum_required(VERSION 3.13)
project(server_tcp C)

set(CMAKE_C_STANDARD 11)
link_libraries(ws2_32 wsock32)

add_executable(server_tcp main.c)