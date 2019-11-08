#include "GLInitializer.h"
#include "GLFunctions.h"

#include <gl/gl.h>
#include <gl/glext.h>
#include <gl/glcorearb.h>

namespace White { 
namespace Engine {
namespace Render { 
namespace GL {

bool GLInitializer::initialized = false;

void GLInitializer::Init() {
    if (initialized) {
        return;
    }

    HMODULE hModule = LoadLibraryW(L"opengl32.dll");
    GLFunctions& gl = GLFunctions::Get();
    gl.Clear = reinterpret_cast<PFNGLCLEARPROC>
               (GetProcAddress(hModule, "glClear"));
    gl.ClearColor = reinterpret_cast<PFNGLCLEARCOLORPROC>
                    (GetProcAddress(hModule, "glClearColor"));
    gl.CreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>
                      (GetProcAddress(hModule, "glCreateShader"));
    gl.ShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>
                      (GetProcAddress(hModule, "glShaderSource"));
    gl.CompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>
                       (GetProcAddress(hModule, "glCompileShader"));
    gl.AttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>
                      (GetProcAddress(hModule, "glAttachShader"));
    gl.DetachShader = reinterpret_cast<PFNGLDETACHSHADERPROC>
                      (GetProcAddress(hModule, "glDetachShader"));
    gl.DeleteShader = reinterpret_cast<PFNGLDELETESHADERPROC>
                      (GetProcAddress(hModule, "glDeleteShader"));
    gl.CreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>
                       (GetProcAddress(hModule, "glCreateProgram"));
    gl.LinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>
                     (GetProcAddress(hModule, "glLinkProgram"));
    gl.UseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>
                    (GetProcAddress(hModule, "glUseProgram"));
    gl.DeleteProgram = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                       (GetProcAddress(hModule, "glDeleteProgram"));

    initialized = true;
}

}
}
}
}
