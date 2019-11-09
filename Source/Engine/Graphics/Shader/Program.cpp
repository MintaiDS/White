#include "Program.h"

namespace White {
namespace Engine {
namespace Graphics {

Program::Program() : id(glCreateProgram()) {}

Program::~Program() {
    glDeleteProgram(id);
}

void Program::Create() {
    id = glCreateProgram();
}

void Program::Attach(const Shader& shader) {
    glAttachShader(id, shader.id);
}

void Program::Detach(const Shader& shader) {
    glDetachShader(id, shader.id);
}

void Program::Link() {
    glLinkProgram(id);
}

void Program::Use() {
    glUseProgram(id);
}

void Program::Delete() {
    glDeleteProgram(id);
}

}
}
}
