#include "BufferObject.h"

namespace White {
namespace Engine {
namespace Graphics {

void BufferObject::Create() {
    glGenBuffers(1, &id);
}

void BufferObject::Bind(GLenum target) {
    this->target = target;
    glBindBuffer(target, id);
}

void BufferObject::SetData(GLsizeiptr size, 
                           const GLvoid* data, GLenum usage) {
    glBufferData(target, size, data, usage);
}

void BufferObject::SetSubData(GLintptr offset, 
                              GLsizeiptr size, const GLvoid* data) {
    glBufferSubData(target, offset, size, data); 
}

void BufferObject::Delete() {
    glDeleteBuffers(1, &id);
}

}
}
}
