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
    Clear = reinterpret_cast<PFNGLCLEARPROC>
            (GetProcAddress(hModule, "glClear"));
    ClearColor = reinterpret_cast<PFNGLCLEARCOLORPROC>
                 (GetProcAddress(hModule, "glClearColor"));
}

}
}
}
}
