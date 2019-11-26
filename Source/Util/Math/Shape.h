#pragma once

#include "Polygon.h"
#include "Mesh.h"

using namespace White::Engine::Graphics;

namespace White {
namespace Util {
namespace Math {

template<typename T, typename U = unsigned>
class Shape {
public:
    virtual Polygon<GLfloat> ToPolygon(int verticesCnt);
    virtual Mesh<T, U> ToMesh(const Vector<T>& color, int verticesCnt);
};

template<typename GLfloat, typename U = unsigned>
Mesh<GLfloat, U> Shape<GLfloat, U>::ToMesh(
                            const Vector<GLfloat>& color, int verticesCnt) {
    std::vector<VertexData<GLfloat>> vertices;
    Mesh<GLfloat, U> ret(vertices);

    return ret;
}

}
}
}
