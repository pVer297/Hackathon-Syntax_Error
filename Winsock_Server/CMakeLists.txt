cmake_minimum_required(VERSION 3.14)
project(Winsock_Server C)

set(CMAKE_C_STANDARD 11)

add_executable(Winsock_Server main.c)

SET(GCC_COVERAGE_LINK_FLAGS "-lws2_32")

target_link_libraries(Winsock_Server ws2_32)