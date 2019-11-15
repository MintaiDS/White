#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

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
    std::string source = buffer.str();
    source += '\0'; 
    file.close();
    GLchar* src = const_cast<GLchar*>(source.data());  
    glShaderSource(id, 1, &src, NULL);
}

void Shader::Compile() {
    glCompileShader(id); 
}

void Shader::Delete() {
    glDeleteShader(id);
}

bool Shader::IsCompiled() {
    GLint param;
    glGetShaderiv(id, GL_COMPILE_STATUS, &param);

    return param == GL_TRUE;
}

int Shader::GetInfoLogLength() {
    GLint param;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &param);

    return param;
}

int Shader::GetSourceLength() {
    GLint param;
    glGetShaderiv(id, GL_SHADER_SOURCE_LENGTH, &param);

    return param;
}

GLenum Shader::GetType() {
    GLint param;
    glGetShaderiv(id, GL_SHADER_TYPE, &param);

    return param;
}

std::string Shader::GetInfoLog() {
    int maxLength = GetInfoLogLength();
    int length;
    std::unique_ptr<GLchar[]> infoLog = std::make_unique<GLchar[]>(maxLength);
    glGetShaderInfoLog(id, maxLength, &length, infoLog.get());

    return std::string(infoLog.get());
}

std::string Shader::GetSource() {
    int bufSize = GetSourceLength();
    int length;
    std::unique_ptr<GLchar[]> source = std::make_unique<GLchar[]>(bufSize);
    glGetShaderSource(id, bufSize, &length, source.get());

    return std::string(source.get());
}

}
}
}
