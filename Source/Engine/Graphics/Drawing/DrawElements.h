#pragma once

#include <windows.h>
#include <gl/gl.h>

namespace White {
namespace Engine {
namespace Render {

struct DrawElements {
    void Invoke();

    GLenum mode;
};

}
}
}
