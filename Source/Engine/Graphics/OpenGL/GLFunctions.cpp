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
FUNCTION_ADDRESS(glDisableVertexAttribArray);
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
FUNCTION_ADDRESS(glGetBufferSubData);
FUNCTION_ADDRESS(glGetBufferParameteriv);
FUNCTION_ADDRESS(glGetUniformLocation);
FUNCTION_ADDRESS(glUniformMatrix4fv);
FUNCTION_ADDRESS(glProgramUniformMatrix4fv);
FUNCTION_ADDRESS(glGetShaderInfoLog);
FUNCTION_ADDRESS(glGetShaderSource);
FUNCTION_ADDRESS(glGetProgramBinary);
FUNCTION_ADDRESS(glGetProgramInfoLog);
FUNCTION_ADDRESS(glGetError);
FUNCTION_ADDRESS(glDisable);
FUNCTION_ADDRESS(glGenerateMipmap);

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
    LOAD_FUNCTION(glDisableVertexAttribArray);
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
    LOAD_FUNCTION(glGetBufferSubData);
    LOAD_FUNCTION(glGetBufferParameteriv);
    LOAD_FUNCTION(glGetUniformLocation);
    LOAD_FUNCTION(glUniformMatrix4fv);
    LOAD_FUNCTION(glProgramUniformMatrix4fv);
    LOAD_FUNCTION(glGetShaderiv);
    LOAD_FUNCTION(glGetShaderInfoLog);
    LOAD_FUNCTION(glGetShaderSource);
    LOAD_FUNCTION(glGetProgramBinary);
    LOAD_FUNCTION(glGetProgramInfoLog);
    LOAD_FUNCTION(glGetError);
    LOAD_FUNCTION(glDisable);
    LOAD_FUNCTION(glGenerateMipmap);
}

}
}
}
