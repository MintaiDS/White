#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace White {
namespace Engine {
namespace Graphics {

Shader::Shader(GLenum shaderType) 
        : id(glCreateShader(shaderType)) {}

Shader::~Shader() {
    glDeleteShader(id);
}

void Shader::Create(GLenum shaderType) {
    id = glCreateShader(shaderType);
}

void Shader::Source(const std::wstring& path) {
    std::ifstream file(path, std::ios::binary); 
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    GLchar* src = const_cast<GLchar*>(buffer.str().c_str());
    glShaderSource(id, 1, &src, 0);
}

void Shader::Compile() {
    glCompileShader(id); 
}

void Shader::Delete() {
    glDeleteShader(id);
}

}
}
}
