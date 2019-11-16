cmake_minimum_required(VERSION 2.6)

set(SRC)
set(SRC ${SRC} GraphVisualizer.cpp) 
set(SRC ${SRC} GraphView.cpp) 
set(SRC ${SRC} Grid.cpp) 
set(SRC ${SRC} Cell.cpp)
set(LIBS)
set(LIBS ${LIBS} Drawing)
set(LIBS ${LIBS} Core)
set(LIBS ${LIBS} Math)
add_library(GraphVisualizer ${SRC})
target_link_libraries(GraphVisualizer ${LIBS})






