cmake_minimum_required(VERSION 2.6)

add_executable(Engine WIN32 Engine.cpp)
target_link_libraries(Engine Canvas)

#target_include_directories(Engine PRIVATE ${INCLUDES})
