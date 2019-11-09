#include "GLFunctions.h"

namespace White {
namespace Engine {
namespace Graphics {
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
