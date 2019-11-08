#pragma once

#include "GLFunctions.h"

namespace White {
namespace Engine {
namespace Render {

struct Program {
    Program();
    ~Program();

    void Create();
    void Attach(const Shader& shader);
    void Detach(const Shader& shader);
    void Link();
    void Use();
    void Delete();

    GLuint id;
};

}
}
}
