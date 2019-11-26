cmake_minimum_required(VERSION 2.6)

set(SRC)
set(SRC ${SRC} Shader.cpp)
set(SRC ${SRC} Program.cpp)
add_library(Shader ${SRC})
