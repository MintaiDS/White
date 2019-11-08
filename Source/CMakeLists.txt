cmake_minimum_required(VERSION 2.6)

project(White)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Util/Math)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Util/System)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Render/Core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Render/GL)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Render/Drawing/Object)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Render/Drawing/Command)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Render/Shader)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Render/Util)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../External/OpenGL)

add_subdirectory(Engine)
add_subdirectory(Util)
