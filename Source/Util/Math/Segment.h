#pragma once

#include "Vector.h"
#include "Polygon.h"
#include "Rectangle.h"
#include "Shape.h"

namespace White {
namespace Util {
namespace Math {

template<typename T, typename U = unsigned>
struct Segment : public Shape<T> {
    Segment(const Vector<T>& begin, const Vector<T>& end);

    virtual Polygon<GLfloat> ToPolygon(int verticesCnt);
    virtual Mesh<T, U> ToMesh(const Vector<T>& color, int verticesCnt);

    Vector<T> begin;
    Vector<T> end;
};

template<typename T, typename U = unsigned>
Segment<T, U>::Segment(const Vector<T>& begin, const Vector<T>& end) 
        : begin(begin)
        , end(end) {}

template<typename GLfloat, typename U = unsigned>
Mesh<GLfloat, U> Segment<GLfloat, U>::ToMesh(
                            const Vector<GLfloat>& color, int verticesCnt) { 
    GLfloat len = (end - begin).Length();
    Rectangle<GLfloat> rect(len, 0.01);

    return rect.ToMesh(color, verticesCnt);
}

}
}
}
