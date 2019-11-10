#pragma once

#include "Shape.h"
#include "Circle.h"

namespace White {
namespace Util {
namespace Math {

template<typename T, typename U = unsigned>
struct Ring : public Shape<T> {
    Ring(const Circle<T>& inner, const Circle<T>& outer);
    Ring(const T radiusInner, const T radiusOuter);

    virtual Polygon<GLfloat> ToPolygon(int verticesCnt);
    virtual Mesh<T, U> ToMesh(const Vector<T>& color, int verticesCnt);
 
    Circle<T> inner;
    Circle<T> outer;
};

template<typename T, typename U = unsigned>
Ring<T, U>::Ring(const Circle<T>& inner, const Circle<T>& outer) 
        : inner(inner)
        , outer(outer) {}

template<typename T, typename U = unsigned>
Ring<T, U>::Ring(const T radiusInner, const T radiusOuter) 
        : inner(radiusInner)
        , outer(radiusOuter) {}

template<typename GLfloat, typename U = unsigned>
Mesh<GLfloat, U> Ring<GLfloat, U>::ToMesh(
                            const Vector<GLfloat>& color, int verticesCnt) {
    Polygon<GLfloat> polygon = ToPolygon(verticesCnt);
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
    for (int i = 2; i < polygon.vertices.size(); i += 2) {
        ret.indices.push_back(i - 2);
        ret.indices.push_back(i);
        ret.indices.push_back(i - 1);
        ret.indices.push_back(i - 1);
        ret.indices.push_back(i);
        ret.indices.push_back(i + 1);
    }

    return ret;
}

}
}
}

