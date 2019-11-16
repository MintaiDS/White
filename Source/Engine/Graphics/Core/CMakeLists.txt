cmake_minimum_required(VERSION 2.6)

add_executable(Engine WIN32 Engine.cpp)
add_library(Core Core.cpp Game.cpp MeshManager.cpp Object.cpp ObjectIDProvider.cpp Pipeline.cpp Renderer.cpp)
target_link_libraries(Engine Context)
target_link_libraries(Engine BufferObject)
target_link_libraries(Engine Program)
target_link_libraries(Engine VertexArrayObject)
target_link_libraries(Engine Shader)
target_link_libraries(Engine Core)
target_link_libraries(Engine GraphVisualizer)














