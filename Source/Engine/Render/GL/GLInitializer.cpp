#include "GLInitializer.h"
#include "GLFunctions.h"

#include <windows.h>

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
}

}
}
}
}
