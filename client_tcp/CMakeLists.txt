cmake_minimum_required(VERSION 3.13)
project(client_tcp C)

set(CMAKE_C_STANDARD 11)
link_libraries(ws2_32 wsock32)

add_executable(client_tcp main.c win_socket2019.h win_socket2019.c)