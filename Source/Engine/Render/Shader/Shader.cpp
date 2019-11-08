#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace White {
namespace Engine {
namespace Render {

Shader::Shader(GLenum shaderType) 
        : id(GL::GLFunctions::Get().CreateShader(shaderType)) {}

Shader::~Shader() {
    GL::GLFunctions::Get().DeleteShader(id);
}

void Shader::Create(GLenum shaderType) {
    id = GL::GLFunctions::Get().CreateShader(shaderType);
}

void Shader::Source(const std::wstring& path) {
    std::ifstream file(path, std::ios::binary); 
    std::stringstream buffer;
    buffer << file.rdbuf();
    GLchar* src = const_cast<GLchar*>(buffer.str().c_str());
    GL::GLFunctions::Get().ShaderSource(id, 1, &src, 0);
}

void Shader::Compile() {
    GL::GLFunctions::Get().CompileShader(id);
}

void Shader::Delete() {
    GL::GLFunctions::Get().DeleteShader(id);
}

}
}
}
