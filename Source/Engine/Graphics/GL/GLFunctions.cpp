#include "GLFunctions.h"

#define DeclareFunction(type, name) \
        typedef type name##_gl_type; \
        name##_gl_type name
#define LoadFunction(name) \
        name = reinterpret_cast<name##_gl_type> \
               (GetFunctionAddress(#name, hModule))

namespace White {
namespace Engine {
namespace Graphics {

DeclareFunction(PFNGLCLEARPROC, glClear);
DeclareFunction(PFNGLCLEARCOLORPROC, glClearColor);
DeclareFunction(PFNGLCREATESHADERPROC, glCreateShader);
DeclareFunction(PFNGLSHADERSOURCEPROC, glShaderSource);
DeclareFunction(PFNGLCOMPILESHADERPROC, glCompileShader); 
DeclareFunction(PFNGLATTACHSHADERPROC, glAttachShader);
DeclareFunction(PFNGLDETACHSHADERPROC, glDetachShader);
DeclareFunction(PFNGLDELETESHADERPROC, glDeleteShader);
DeclareFunction(PFNGLCREATEPROGRAMPROC, glCreateProgram);
DeclareFunction(PFNGLLINKPROGRAMPROC, glLinkProgram);
DeclareFunction(PFNGLUSEPROGRAMPROC, glUseProgram);
DeclareFunction(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
DeclareFunction(PFNGLGENBUFFERSPROC, glGenBuffers);
DeclareFunction(PFNGLBINDBUFFERPROC, glBindBuffer);
DeclareFunction(PFNGLBUFFERDATAPROC, glBufferData);
DeclareFunction(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
DeclareFunction(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
DeclareFunction(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
DeclareFunction(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
DeclareFunction(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
DeclareFunction(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
DeclareFunction(PFNGLENABLEPROC, glEnable);
DeclareFunction(PFNGLCULLFACEPROC, glCullFace);
DeclareFunction(PFNGLFRONTFACEPROC, glFrontFace);
DeclareFunction(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
DeclareFunction(PFNGLGETSHADERIVPROC, glGetShaderiv);
DeclareFunction(PFNGLGETPROGRAMIVPROC, glGetProgramiv);

void* GetFunctionAddress(char* name, HMODULE hModule) {
    PROC address = wglGetProcAddress(name);
    if (!address) {
        address = GetProcAddress(hModule, name);
    }

    return address;
}

void LoadFunctions() {
    HMODULE hModule = LoadLibraryW(L"opengl32.dll");
    LoadFunction(glClear);
    LoadFunction(glClearColor);
    LoadFunction(glCreateShader);
    LoadFunction(glShaderSource);
    LoadFunction(glCompileShader); 
    LoadFunction(glAttachShader);
    LoadFunction(glDetachShader);
    LoadFunction(glDeleteShader);
    LoadFunction(glCreateProgram);
    LoadFunction(glLinkProgram);
    LoadFunction(glUseProgram);
    LoadFunction(glDeleteProgram);
    LoadFunction(glGenBuffers);
    LoadFunction(glBindBuffer);
    LoadFunction(glBufferData);
    LoadFunction(glVertexAttribPointer);
    LoadFunction(glEnableVertexAttribArray);
    LoadFunction(glBufferSubData);
    LoadFunction(glDeleteBuffers);
    LoadFunction(glDeleteVertexArrays);
    LoadFunction(glGenVertexArrays);
    LoadFunction(glEnable);
    LoadFunction(glCullFace);
    LoadFunction(glFrontFace);
    LoadFunction(glBindVertexArray);
    LoadFunction(glGetShaderiv);
    LoadFunction(glGetProgramiv); 
}

}
}
}
