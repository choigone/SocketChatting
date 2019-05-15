cmake_minimum_required(VERSION 3.13)
project(Socket_c C)

set(CMAKE_C_STANDARD 99)

add_executable(Socket_c server.c client.c header.h)