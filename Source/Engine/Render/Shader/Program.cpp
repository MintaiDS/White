#include "Program.h"

#include <fstream>

namespace White {
namespace Engine {
namespace Render {

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
    GLint ret;
    GL::GLFunctions::Get().GetProgramiv(id, GL_LINK_STATUS, &ret);
    std::ofstream out("log.txt");
    out << ret;
    out.close();
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
