cmake_minimum_required(VERSION 2.6)

set(SRC)
set(SRC ${SRC} Thread.cpp) 
set(SRC ${SRC} Mutex.cpp) 
add_library(Threading ${SRC})
