#pragma once

#include "GLFunctions.h"

namespace White {
namespace Engine {
namespace Graphics {

class VertexArrayObject {
public:
    void Create();
    void Bind();
    void Delete();

private:
    GLuint id;
};

}
}
}
