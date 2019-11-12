#pragma once

#include <windows.h>

#include <gl/gl.h>
#include <gl/glext.h>
#include <gl/glcorearb.h>

#define EXTERN_FUNCTION_ADDRESS(type, name) \
        typedef type name##_gl_type; \
        extern name##_gl_type name

namespace White {
namespace Engine {
namespace Graphics {

EXTERN_FUNCTION_ADDRESS(PFNGLCLEARPROC, 
                        glClear);
EXTERN_FUNCTION_ADDRESS(PFNGLCLEARCOLORPROC, 
                        glClearColor);
EXTERN_FUNCTION_ADDRESS(PFNGLCREATESHADERPROC, 
                        glCreateShader);
EXTERN_FUNCTION_ADDRESS(PFNGLSHADERSOURCEPROC, 
                        glShaderSource);
EXTERN_FUNCTION_ADDRESS(PFNGLCOMPILESHADERPROC, 
                        glCompileShader); 
EXTERN_FUNCTION_ADDRESS(PFNGLATTACHSHADERPROC, 
                        glAttachShader);
EXTERN_FUNCTION_ADDRESS(PFNGLDETACHSHADERPROC, 
                        glDetachShader);
EXTERN_FUNCTION_ADDRESS(PFNGLDELETESHADERPROC, 
                        glDeleteShader);
EXTERN_FUNCTION_ADDRESS(PFNGLCREATEPROGRAMPROC, 
                        glCreateProgram);
EXTERN_FUNCTION_ADDRESS(PFNGLLINKPROGRAMPROC, 
                        glLinkProgram);
EXTERN_FUNCTION_ADDRESS(PFNGLUSEPROGRAMPROC, 
                        glUseProgram);
EXTERN_FUNCTION_ADDRESS(PFNGLDELETEPROGRAMPROC, 
                        glDeleteProgram);
EXTERN_FUNCTION_ADDRESS(PFNGLGENBUFFERSPROC, 
                        glGenBuffers);
EXTERN_FUNCTION_ADDRESS(PFNGLBINDBUFFERPROC, 
                        glBindBuffer);
EXTERN_FUNCTION_ADDRESS(PFNGLBUFFERDATAPROC, 
                        glBufferData);
EXTERN_FUNCTION_ADDRESS(PFNGLVERTEXATTRIBPOINTERPROC, 
                        glVertexAttribPointer);
EXTERN_FUNCTION_ADDRESS(PFNGLENABLEVERTEXATTRIBARRAYPROC, 
                        glEnableVertexAttribArray);
EXTERN_FUNCTION_ADDRESS(PFNGLBUFFERSUBDATAPROC, 
                        glBufferSubData);
EXTERN_FUNCTION_ADDRESS(PFNGLDELETEBUFFERSPROC, 
                        glDeleteBuffers);
EXTERN_FUNCTION_ADDRESS(PFNGLDELETEVERTEXARRAYSPROC, 
                        glDeleteVertexArrays);
EXTERN_FUNCTION_ADDRESS(PFNGLGENVERTEXARRAYSPROC, 
                        glGenVertexArrays);
EXTERN_FUNCTION_ADDRESS(PFNGLENABLEPROC, 
                        glEnable);
EXTERN_FUNCTION_ADDRESS(PFNGLCULLFACEPROC, 
                        glCullFace);
EXTERN_FUNCTION_ADDRESS(PFNGLFRONTFACEPROC, 
                        glFrontFace);
EXTERN_FUNCTION_ADDRESS(PFNGLBINDVERTEXARRAYPROC, 
                        glBindVertexArray);
EXTERN_FUNCTION_ADDRESS(PFNGLGETSHADERIVPROC, 
                        glGetShaderiv);
EXTERN_FUNCTION_ADDRESS(PFNGLGETPROGRAMIVPROC, 
                        glGetProgramiv);
EXTERN_FUNCTION_ADDRESS(PFNGLGETBUFFERSUBDATAPROC, 
                        glGetBufferSubData);
EXTERN_FUNCTION_ADDRESS(PFNGLGETBUFFERPARAMETERIVPROC,
                        glGetBufferParameteriv);
EXTERN_FUNCTION_ADDRESS(PFNGLGETUNIFORMLOCATIONPROC, 
                        glGetUniformLocation);
EXTERN_FUNCTION_ADDRESS(PFNGLUNIFORMMATRIX4FVPROC, 
                        glUniformMatrix4fv);

void* GetFunctionAddress(char* name, HMODULE hModule);
void LoadFunctions();

}
}
}
