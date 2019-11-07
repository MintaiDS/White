#pragma once

#include <windows.h>

#include <gl/gl.h>
#include <gl/glext.h>
#include <gl/glcorearb.h>

namespace White {
namespace Engine {
namespace Render {
namespace GL {

struct GLFunctions {
    GLFunctions(const GLFunctions& other) = delete;
    void operator=(const GLFunctions& other) = delete;
    static GLFunctions& Get();

    PFNGLCLEARPROC Clear;
    PFNGLCLEARCOLORPROC ClearColor;

private:
    GLFunctions();
};

}
}
}
}
