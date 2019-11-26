#include "VertexArrayObject.h"

namespace White {
namespace Engine {
namespace Graphics {

void VertexArrayObject::Create() {
    glGenVertexArrays(1, &id);
}

void VertexArrayObject::Bind() {
    glBindVertexArray(id);
}

void VertexArrayObject::Delete() {
    glDeleteVertexArrays(1, &id);
}

}
}
}

