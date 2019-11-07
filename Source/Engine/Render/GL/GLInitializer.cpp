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
    HMODULE hModule = LoadLibraryW(L"opengl32.dll");
    GLFunctions& gl = GLFunctions::Get();
    gl.Clear = reinterpret_cast<PFNGLCLEARPROC>
               (GetProcAddress(hModule, "glClear"));
    gl.ClearColor = reinterpret_cast<PFNGLCLEARCOLORPROC>
                    (GetProcAddress(hModule, "glClearColor"));
    gl.CreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>
                      (GetProcAddress(hModule, "glCreateShader"));
    gl.CompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>
                       (GetProcAddress(hModule, "glCompileShader"));
    gl.CreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>
                       (GetProcAddress(hModule, "glCreateProgram"));
    gl.UseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>
                    (GetProcAddress(hModule, "glUseProgram"));
}

}
}
}
}
