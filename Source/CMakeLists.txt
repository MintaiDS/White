cmake_minimum_required(VERSION 2.6)

include(ExternalProject)
set(JSON_INCLUDE_PATH External/nlohmann/src/json/single_include/nlohmann)
set(JSON_PATH External/nlohmann)
ExternalProject_Add(json
    GIT_REPOSITORY https://github.com/nlohmann/json
    DOWNLOAD_COMMAND git clone --depth 1 https://github.com/nlohmann/json
    GIT_TAG v3.7.2
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../${JSON_PATH}
)
set(EGL_INCLUDE_PATH External/KhronosGroup/EGL-Registry/src/EGL-Registry/api)
set(EGL_PATH External/KhronosGroup/EGL-Registry)
ExternalProject_Add(EGL-Registry
    GIT_REPOSITORY https://github.com/KhronosGroup/EGL-Registry
    DOWNLOAD_COMMAND git clone --depth 1 https://github.com/KhronosGroup/EGL-Registry
    GIT_TAG master
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../${EGL_PATH}
)
set(OPENGL_INCLUDE_PATH External/KhronosGroup/OpenGL-Registry/src/OpenGL-Registry/api)
set(OPENGL_PATH External/KhronosGroup/OpenGL-Registry)
ExternalProject_Add(OpenGL-Registry
    GIT_REPOSITORY https://github.com/KhronosGroup/OpenGL-Registry
    DOWNLOAD_COMMAND git clone --depth 1 https://github.com/KhronosGroup/OpenGL-Registry
    GIT_TAG master
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../${OPENGL_PATH}
)
set(TEST_PATH External/WGForgeTestData)
ExternalProject_Add(WGForgeTestData
    GIT_REPOSITORY https://github.com/glisquery/WGForgeTestData
    DOWNLOAD_COMMAND git clone --depth 1 https://github.com/glisquery/WGForgeTestData
    GIT_TAG master
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../${TEST_PATH}
)
project(White)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Util/Math)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Util/System)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Graphics/Core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Graphics/OpenGL)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Graphics/Drawing/Object)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Graphics/Drawing/Command)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Graphics/Drawing)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Graphics/Shader)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Graphics/Interface)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Graphics/Util)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/GraphVisualizer)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../External/OpenGL)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../${JSON_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../${EGL_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../${OPENGL_INCLUDE_PATH})
add_subdirectory(Engine)
add_subdirectory(Util)
add_subdirectory(GraphVisualizer)
