cmake_minimum_required(VERSION 2.6)

set(SRC)
set(SRC ${SRC} Window.cpp)
set(SRC ${SRC} Logger.cpp)
add_library(System ${SRC})

