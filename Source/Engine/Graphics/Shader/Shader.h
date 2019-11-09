#pragma once

#include "GLFunctions.h"

#include <string>

namespace White {
namespace Engine {
namespace Graphics {

struct Shader {
    Shader(GLenum shaderType = GL_VERTEX_SHADER);
    ~Shader(); 
    
    void Create(GLenum shaderType = GL_VERTEX_SHADER);
    void Source(const std::wstring& path);
    void Compile();
    void Delete();

    GLuint id;
};

}
}
}
