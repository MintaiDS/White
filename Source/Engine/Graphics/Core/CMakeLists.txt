cmake_minimum_required(VERSION 2.6)

set(SRC)
set(SRC ${SRC} Core.cpp) 
set(SRC ${SRC} Game.cpp) 
set(SRC ${SRC} MeshManager.cpp) 
set(SRC ${SRC} Object.cpp) 
set(SRC ${SRC} ObjectIDProvider.cpp) 
set(SRC ${SRC} Pipeline.cpp) 
set(SRC ${SRC} Renderer.cpp)
set(LIBS)
set(LIBS ${LIBS} Core)
set(LIBS ${LIBS} OpenGL)
set(LIBS ${LIBS} Shader)
set(LIBS ${LIBS} Util)
set(LIBS ${LIBS} Math)
set(LIBS ${LIBS} GraphVisualizer)
add_executable(Engine WIN32 Engine.cpp)
add_library(Core ${SRC})
target_link_libraries(Engine ${LIBS})















