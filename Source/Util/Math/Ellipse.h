#pragma once

#include "Shape.h"

namespace White {
namespace Util {
namespace Math {

template<typename T, typename U = unsigned>
struct Ellipse : public Shape<T> {
    Ellipse(const T w, const T h);

    virtual Polygon<GLfloat> ToPolygon(int verticesCnt);
    virtual Mesh<T, U> ToMesh(const Vector<T>& color, int verticesCnt);

    T w;
    T h;
};

template<typename T, typename U = unsigned>
Ellipse<T, U>::Ellipse(const T w, const T h) : w(w), h(h) {}

template<typename GLfloat, typename U = unsigned>
Mesh<GLfloat, U> Ellipse<GLfloat, U>::ToMesh(
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

    return ret;
}

}
}
}

