#include "Program.h"

namespace White {
namespace Engine {
namespace Graphics {

Program::Program() : id(0) {}

Program::~Program() {}

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

int Program::GetVariableLocation(std::string name) {
    return glGetUniformLocation(id, name.c_str());
}

void Program::SetVariable(std::string name, float v0) {
}

void Program::SetVariable(std::string name, float v0, float v1) {
}

void Program::SetVariable(std::string name, float v0, float v1, float v2) {
}

void Program::SetVariable(std::string name, float v0, 
                          float v1, float v2, float v3) {
}

void Program::SetVariable(std::string name, int v0) {
}

void Program::SetVariable(std::string name, int v0, int v1) {
}

void Program::SetVariable(std::string name, int v0, int v1, int v2) {
}

void Program::SetVariable(std::string name, int v0, int v1, int v2, int v3) {
}

void Program::SetVariable(std::string name, unsigned v0) {
}

void Program::SetVariable(std::string name, unsigned v0, unsigned v1) {
}

void Program::SetVariable(std::string name, 
                          unsigned v0, unsigned v1, unsigned v2) {
}

void Program::SetVariable(std::string name, unsigned v0, 
                          unsigned v1, unsigned v2, unsigned v3) {
}

void Program::SetVariable(int location, float v0) {
}

void Program::SetVariable(int location, float v0, float v1) {
}

void Program::SetVariable(int location, float v0, float v1, float v2) {
}

void Program::SetVariable(int location, float v0, 
                          float v1, float v2, float v3) {
}

void Program::SetVariable(int location, int v0) {
}

void Program::SetVariable(int location, int v0, int v1) {
}

void Program::SetVariable(int location, int v0, int v1, int v2) {
}

void Program::SetVariable(int location, int v0, int v1, int v2, int v3) {
}

void Program::SetVariable(int location, unsigned v0) {
}

void Program::SetVariable(int location, unsigned v0, unsigned v1) {
}

void Program::SetVariable(int location, unsigned v0, 
                          unsigned v1, unsigned v2) {
}

void Program::SetVariable(int location, unsigned v0, 
                          unsigned v1, unsigned v2, unsigned v3) {
}



}
}
}
