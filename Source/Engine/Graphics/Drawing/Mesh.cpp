#include "Mesh.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Render {

Mesh::Mesh(const Mesh& other) : vertices(other.vertices) {}

Mesh::Mesh(const std::vector<Vector<GLfloat>>& vertices) : vertices(vertices) {}

Mesh::Mesh(const std::size_t size, const Vector<GLfloat>* vertices) {
    for (int i = 0; i < size; i++) {
        this->vertices.push_back(vertices[i]);
    }
}

}
}
}
