#include "GLFunctions.h"

namespace White {
namespace Engine {
namespace Render {
namespace GL {

GLFunctions& GLFunctions::Get() {
    static GLFunctions instance;

    return instance;
}

GLFunctions::GLFunctions() {}

}
}
}
}
