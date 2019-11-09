#include "GLFunctions.h"

#define LoadFunction(type, name, module) \
        name = reinterpret_cast<type>(GetFunctionAddress(#name, hModule))

namespace White {
namespace Engine {
namespace Graphics {

PFNGLCLEARPROC glClear;
PFNGLCLEARCOLORPROC glClearColor;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader; 
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLENABLEPROC glEnable;
PFNGLCULLFACEPROC glCullFace;
PFNGLFRONTFACEPROC glFrontFace;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETPROGRAMIVPROC glGetProgramiv;

void* GetFunctionAddress(char* name, HMODULE hModule) {
    PROC address = wglGetProcAddress(name);
    if (!address) {
        address = GetProcAddress(hModule, name);
    }

    return address;
}

void LoadFunctions() {
    HMODULE hModule = LoadLibraryW(L"opengl32.dll");
    LoadFunction(PFNGLCLEARPROC, glClear);
    LoadFunction(PFNGLCLEARCOLORPROC, glClearColor);
    LoadFunction(PFNGLCREATESHADERPROC, glCreateShader);
    LoadFunction(PFNGLSHADERSOURCEPROC, glShaderSource);
    LoadFunction(PFNGLCOMPILESHADERPROC, glCompileShader); 
    LoadFunction(PFNGLATTACHSHADERPROC, glAttachShader);
    LoadFunction(PFNGLDETACHSHADERPROC, glDetachShader);
    LoadFunction(PFNGLDELETESHADERPROC, glDeleteShader);
    LoadFunction(PFNGLCREATEPROGRAMPROC, glCreateProgram);
    LoadFunction(PFNGLLINKPROGRAMPROC, glLinkProgram);
    LoadFunction(PFNGLUSEPROGRAMPROC, glUseProgram);
    LoadFunction(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
    LoadFunction(PFNGLGENBUFFERSPROC, glGenBuffers);
    LoadFunction(PFNGLBINDBUFFERPROC, glBindBuffer);
    LoadFunction(PFNGLBUFFERDATAPROC, glBufferData);
    LoadFunction(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
    LoadFunction(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
    LoadFunction(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
    LoadFunction(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
    LoadFunction(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
    LoadFunction(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
    LoadFunction(PFNGLENABLEPROC, glEnable);
    LoadFunction(PFNGLCULLFACEPROC, glCullFace);
    LoadFunction(PFNGLFRONTFACEPROC, glFrontFace);
    LoadFunction(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
    LoadFunction(PFNGLGETSHADERIVPROC, glGetShaderiv);
    LoadFunction(PFNGLGETPROGRAMIVPROC, glGetProgramiv); 
}

}
}
}
