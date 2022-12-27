cmake_minimum_required(VERSION 3.23)
project(tp5 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(tp5
        product.h product.c)