cmake_minimum_required(VERSION 2.6)

add_library(GraphVisualizer GraphVisualizer.cpp GraphView.cpp Grid.cpp Cell.cpp)
target_link_libraries(GraphVisualizer Drawing Core Math)






