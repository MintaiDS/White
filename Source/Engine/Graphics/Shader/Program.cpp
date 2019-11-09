#include "Program.h"

namespace White {
namespace Engine {
namespace Graphics {

Program::Program() : id(GL::GLFunctions::Get().CreateProgram()) {}

Program::~Program() {
    GL::GLFunctions::Get().DeleteProgram(id);
}

void Program::Create() {
    id = GL::GLFunctions::Get().CreateProgram();
}

void Program::Attach(const Shader& shader) {
    GL::GLFunctions::Get().AttachShader(id, shader.id);
}

void Program::Detach(const Shader& shader) {
    GL::GLFunctions::Get().DetachShader(id, shader.id);
}

void Program::Link() {
    GL::GLFunctions::Get().LinkProgram(id);
}

void Program::Use() {
    GL::GLFunctions::Get().UseProgram(id);
}

void Program::Delete() {
    GL::GLFunctions::Get().DeleteProgram(id);
}

}
}
}
