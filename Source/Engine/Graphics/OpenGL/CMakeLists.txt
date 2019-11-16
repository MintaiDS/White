cmake_minimum_required(VERSION 2.6)

set(SRC)
set(SRC ${SRC} GLFunctions.cpp)
set(SRC ${SRC} GLInitializer.cpp)
add_library(OpenGL ${SRC})
