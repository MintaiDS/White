#pragma once

#include "Circle.h"

#include <iostream>
#include <fstream>

namespace White {
namespace Util {
namespace Math {

template<typename T, typename U = unsigned>
struct Disk : public Circle<T> {
    Disk(T r);

    virtual Polygon<GLfloat> ToPolygon(int verticesCnt);
    virtual Mesh<T, U> ToMesh(const Vector<T>& color, int verticesCnt);
};

template<typename T, typename U = unsigned>
Disk<T, U>::Disk(T r) : Circle(r) {}

template<typename GLfloat, typename U = unsigned>
Mesh<GLfloat, U> Disk<GLfloat, U>::ToMesh(
                            const Vector<GLfloat>& color, int verticesCnt) {
    Util::Math::Polygon<GLfloat> polygon = ToPolygon(verticesCnt); 
    std::vector<VertexData<GLfloat>> vertices;
    VertexData<GLfloat> vertex;
    vertex.attributes.push_back(Vector<GLfloat>({0.0f, 0.0f, 0.0f, 1.0f}));
    vertex.attributes.push_back(color);
    vertices.push_back(vertex);
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
    for (int i = 1; i < polygon.vertices.size(); i++) {
        ret.indices.push_back(0);
        ret.indices.push_back(i + 1);
        ret.indices.push_back(i);
    }
   
    return ret;
}

}
}
}

