#include "BufferObject.h"
#include "GLFunctions.h"

namespace White {
namespace Engine {
namespace Grahpics {

void BufferObject::Create() {
    GL::GLFunctions& gl = GL::GLFunctions::Get();
    gl.GenBuffers(1, &id);
}

void BufferObject::Bind(GLenum target) {
    this->target = target;
    GL::GLFunctions& gl = GL::GLFunctions::Get();
    gl.BindBuffer(target, id);
}

void BufferObject::SetData(GLsizeiptr size, 
                           const GLvoid* data, GLenum usage) {
    GL::GLFunctions& gl = GL::GLFunctions::Get();
    gl.BufferData(target, size, data);
}

void BufferObject::Delete() {
    GL::GLFunctions& gl = GL::GLFunctions::Get();
    glDeleteBuffers(1, &id);
}

}
}
}
