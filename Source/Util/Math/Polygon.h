#pragma once

#include "GLFunctions.h"
#include "Vector.h"
#include "Ellipse.h"
#include "Disk.h"
#include "Ring.h"
#include "Rectangle.h"
#include "Segment.h"
#include "Common.h"

namespace White {
namespace Util {
namespace Math {

template<typename T>
struct Polygon {
    Polygon(const std::vector<Vector<T>>& vertices);

    static Polygon<GLfloat> CreateFromShape(
                                const Ellipse<GLfloat>& ellipse, int verticesCnt);
    static Polygon<GLfloat> CreateFromShape(
                               const Disk<GLfloat>& disk, int verticesCnt);
    static Polygon<GLfloat> CreateFromShape(
                               const Ring<GLfloat>& ring, int verticesCnt);
    static Polygon<GLfloat> CreateFromShape(const Rectangle<GLfloat>& rectangle);
    static Polygon<GLfloat> CreateFromShape(const Segment<GLfloat>& segment);

    void Add(const Vector<T>& vertex);

    std::vector<Vector<T>> vertices;
};

template<typename T>
Polygon<T>::Polygon(const std::vector<Vector<T>>& vertices) 
        : vertices(vertices) {}

template<typename T>
void Polygon<T>::Add(const Vector<T>& vertex) {
    vertices.push_back(vertex);
}

}
}
}
