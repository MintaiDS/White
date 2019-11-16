cmake_minimum_required(VERSION 2.6)

add_executable(Engine WIN32 Engine.cpp)
add_library(Core Core.cpp Game.cpp MeshManager.cpp Object.cpp ObjectIDProvider.cpp Pipeline.cpp Renderer.cpp)
target_link_libraries(Engine Core)
target_link_libraries(Engine OpenGL)
target_link_libraries(Engine Shader)
target_link_libraries(Engine Util)
target_link_libraries(Engine Math)
target_link_libraries(Engine GraphVisualizer)














