#pragma once

#include "Shape.h"

namespace White {
namespace Util {
namespace Math {

template<typename T, typename U = unsigned>
struct Rectangle : public Shape<T> {
    Rectangle(const T w, const T h);

    Mesh<T, U> ToMesh(const Vector<T>& color, int verticesCnt);
    virtual Polygon<T> ToPolygon(int verticesCnt);

    T w;
    T h;
};

template<typename T, typename U = unsigned>
Rectangle<T, U>::Rectangle(const T w, const T h) : w(w), h(h) {}

template<typename GLfloat, typename U = unsigned>
Mesh<GLfloat, U> Rectangle<GLfloat, U>::ToMesh(
                          const Vector<GLfloat>& color, int verticesCnt) {
    Util::Math::Polygon<GLfloat> polygon = ToPolygon(verticesCnt);
    std::vector<VertexData<GLfloat>> vertices;
    for (int i = 0; i < polygon.vertices.size(); i++) {
        VertexData<GLfloat> vertex;
        Vector<GLfloat> position = {polygon.vertices[i][0], 
                                    polygon.vertices[i][1],
                                    0, 1};
        vertex.attributes.push_back(position);
        vertex.attributes.push_back(color);
        vertices.push_back(vertex);
    }
    Mesh<GLfloat, U> ret(vertices);
    ret.indices.push_back(0);
    ret.indices.push_back(1);
    ret.indices.push_back(2);
    ret.indices.push_back(2);
    ret.indices.push_back(3);
    ret.indices.push_back(0);

    return ret;
}

}
}
}

