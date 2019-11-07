#include "Program.h"

namespace White {
namespace Engine {
namespace Render {

Program::Program() : id(GLFunctions::Get().CreateProgram()) {
}

Program::~Program() {
}

}
}
}
