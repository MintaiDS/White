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

void BufferObject::Delete() {
    glDeleteBuffers(1, &id);
}

void BufferObject::SetData(GLsizeiptr size, 
                           const GLvoid* data, GLenum usage) {
    glBufferData(target, size, data, usage);
}

void BufferObject::SetSubData(GLintptr offset, 
                              GLsizeiptr size, const GLvoid* data) {
    glBufferSubData(target, offset, size, data); 
}

void BufferObject::GetSubData(GLintptr offset, 
                              GLsizeiptr size, GLvoid* data) {
    glGetBufferSubData(target, offset, size, data);
}

GLint BufferObject::GetSize() {
    GLint ret;
    glGetBufferParameteriv(target, GL_BUFFER_SIZE, &ret);

    return ret;
}

GLint BufferObject::GetUsage() {
    GLint ret;
    glGetBufferParameteriv(target, GL_BUFFER_USAGE, &ret);

    return ret;
}

GLenum BufferObject::GetTarget() {
    return target;
}

}
}
}
