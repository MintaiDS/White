#include "BufferObject.h"
#include "GLFunctions.h"

namespace White {
namespace Engine {
namespace Grahpics {

void BufferObject::Create() {
    glGenBuffers(1, &id);
}

void BufferObject::Bind(GLenum target) {
    this->target = target;
    glBindBuffer(target, id);
}

void BufferObject::SetData(GLsizeiptr size, 
                           const GLvoid* data, GLenum usage) {
    glBufferData(target, size, data);
}

void BufferObject::Delete() {
    glDeleteBuffers(1, &id);
}

}
}
}
