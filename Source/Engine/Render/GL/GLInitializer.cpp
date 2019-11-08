#include "GLInitializer.h"
#include "GLFunctions.h"

#include <gl/gl.h>
#include <gl/glext.h>
#include <gl/glcorearb.h>

namespace White { 
namespace Engine {
namespace Render { 
namespace GL {

void GLInitializer::Init() {
    GLFunctions& gl = GLFunctions::Get();
    gl.Clear = reinterpret_cast<PFNGLCLEARPROC>
               (GetFunctionAddress("glClear"));
    gl.ClearColor = reinterpret_cast<PFNGLCLEARCOLORPROC>
                    (GetFunctionAddress("glClearColor"));
    gl.CreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>
                      (GetFunctionAddress("glCreateShader"));
    gl.ShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>
                      (GetFunctionAddress("glShaderSource"));
    gl.CompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>
                       (GetFunctionAddress("glCompileShader"));
    gl.AttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>
                      (GetFunctionAddress("glAttachShader"));
    gl.DetachShader = reinterpret_cast<PFNGLDETACHSHADERPROC>
                      (GetFunctionAddress("glDetachShader"));
    gl.DeleteShader = reinterpret_cast<PFNGLDELETESHADERPROC>
                      (GetFunctionAddress("glDeleteShader"));
    gl.CreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>
                       (GetFunctionAddress("glCreateProgram"));
    gl.LinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>
                     (GetFunctionAddress("glLinkProgram"));
    gl.UseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>
                    (GetFunctionAddress("glUseProgram"));
    gl.DeleteProgram = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                       (GetFunctionAddress("glDeleteProgram"));
}

void* GLInitializer::GetFunctionAddress(char* name) {
    PROC address = wglGetProcAddress(name);
    if (!address) {
        HMODULE hModule = LoadLibraryW(L"opengl32.dll");
        address = GetProcAddress(hModule, name);
    }

    return address;
}

}
}
}
}
