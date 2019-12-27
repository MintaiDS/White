#pragma once

#include "GLFunctions.h"

#include <string>

namespace White {
namespace Engine {
namespace Graphics {

struct Shader {
    Shader();
    ~Shader(); 
    
    void Create(GLenum shaderType = GL_VERTEX_SHADER);
    void Source(const std::wstring& path);
    void Compile();
    void Delete();
    bool IsCompiled();
    int GetInfoLogLength();
    int GetSourceLength();
    GLenum GetType();
    std::string GetInfoLog();
    std::string GetSource();

    GLuint id;
};

}
}
}
