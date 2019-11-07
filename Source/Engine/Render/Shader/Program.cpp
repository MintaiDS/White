#include "Program.h"

namespace White {
namespace Engine {
namespace Render {

Program::Program() : id(GL::GLFunctions::Get().CreateProgram()) {}

Program::~Program() {
    GL::GLFunctions::Get().DeleteProgram(id);
}

}
}
}
