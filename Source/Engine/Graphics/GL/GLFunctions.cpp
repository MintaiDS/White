#include "GLFunctions.h"

#define FUNCTION_ADDRESS(name) \
        name##_gl_type name
#define LOAD_FUNCTION(name) \
        name = reinterpret_cast<name##_gl_type> \
               (GetFunctionAddress(#name, hModule))

namespace White {
namespace Engine {
namespace Graphics {

FUNCTION_ADDRESS(glClear);
FUNCTION_ADDRESS(glClearColor);
FUNCTION_ADDRESS(glCreateShader);
FUNCTION_ADDRESS(glShaderSource);
FUNCTION_ADDRESS(glCompileShader); 
FUNCTION_ADDRESS(glAttachShader);
FUNCTION_ADDRESS(glDetachShader);
FUNCTION_ADDRESS(glDeleteShader);
FUNCTION_ADDRESS(glCreateProgram);
FUNCTION_ADDRESS(glLinkProgram);
FUNCTION_ADDRESS(glUseProgram);
FUNCTION_ADDRESS(glDeleteProgram);
FUNCTION_ADDRESS(glGenBuffers);
FUNCTION_ADDRESS(glBindBuffer);
FUNCTION_ADDRESS(glBufferData);
FUNCTION_ADDRESS(glVertexAttribPointer);
FUNCTION_ADDRESS(glEnableVertexAttribArray);
FUNCTION_ADDRESS(glBufferSubData);
FUNCTION_ADDRESS(glDeleteBuffers);
FUNCTION_ADDRESS(glDeleteVertexArrays);
FUNCTION_ADDRESS(glGenVertexArrays);
FUNCTION_ADDRESS(glEnable);
FUNCTION_ADDRESS(glCullFace);
FUNCTION_ADDRESS(glFrontFace);
FUNCTION_ADDRESS(glBindVertexArray);
FUNCTION_ADDRESS(glGetShaderiv);
FUNCTION_ADDRESS(glGetProgramiv);

void* GetFunctionAddress(char* name, HMODULE hModule) {
    PROC address = wglGetProcAddress(name);
    if (!address) {
        address = GetProcAddress(hModule, name);
    }

    return address;
}

void LoadFunctions() {
    HMODULE hModule = LoadLibraryW(L"opengl32.dll");
    LOAD_FUNCTION(glClear);
    LOAD_FUNCTION(glClearColor);
    LOAD_FUNCTION(glCreateShader);
    LOAD_FUNCTION(glShaderSource);
    LOAD_FUNCTION(glCompileShader); 
    LOAD_FUNCTION(glAttachShader);
    LOAD_FUNCTION(glDetachShader);
    LOAD_FUNCTION(glDeleteShader);
    LOAD_FUNCTION(glCreateProgram);
    LOAD_FUNCTION(glLinkProgram);
    LOAD_FUNCTION(glUseProgram);
    LOAD_FUNCTION(glDeleteProgram);
    LOAD_FUNCTION(glGenBuffers);
    LOAD_FUNCTION(glBindBuffer);
    LOAD_FUNCTION(glBufferData);
    LOAD_FUNCTION(glVertexAttribPointer);
    LOAD_FUNCTION(glEnableVertexAttribArray);
    LOAD_FUNCTION(glBufferSubData);
    LOAD_FUNCTION(glDeleteBuffers);
    LOAD_FUNCTION(glDeleteVertexArrays);
    LOAD_FUNCTION(glGenVertexArrays);
    LOAD_FUNCTION(glEnable);
    LOAD_FUNCTION(glCullFace);
    LOAD_FUNCTION(glFrontFace);
    LOAD_FUNCTION(glBindVertexArray);
    LOAD_FUNCTION(glGetShaderiv);
    LOAD_FUNCTION(glGetProgramiv); 
}

}
}
}
