#pragma once

namespace White {
namespace Engine {
namespace Graphics {

struct BufferObject {
    void Create();
    void Bind();
    void SetData(GLsizeiptr size, const GLvoid* data, GLenum usage);
    void SetSubData(GLsizeiptr size, GLintptr offset, 
                    GLsizeiptr size, const GLvoid* data); 
    void Delete();

    GLuint id;
    GLenum target;
};

}
}
}
