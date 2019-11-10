#include "Polygon.h"

namespace White {
namespace Util {
namespace Math {

template<>
Polygon<GLfloat> Polygon<GLfloat>::CreateFromShape(
                                const Ellipse<GLfloat>& ellipse, int verticesCnt) {
    std::vector<Vector<GLfloat>> vertices;
    GLfloat angle = 0.0f;
    Vector<GLfloat> vertex = {ellipse.w / 2.0f, 0.0f};
    for (int i = 0; i < verticesCnt; i++) {
        vertices.push_back(vertex);
        angle += 360.0f / verticesCnt;
        vertex = {(ellipse.w / 2.0f) * std::cos(ToRadians(angle)), 
                  (ellipse.h / 2.0f) * std::sin(ToRadians(angle))};
    }
    Polygon<GLfloat> ret(vertices);

    return ret;
}

template<>
Polygon<GLfloat> Polygon<GLfloat>::CreateFromShape(
                                const Disk<GLfloat>& disk, int verticesCnt) {
    std::vector<Vector<GLfloat>> vertices;
    GLfloat angle = 0.0f;
    Vector<GLfloat> vertex = {disk.w / 2.0f, 0.0f};
    for (int i = 1; i < verticesCnt; i++) {
        vertices.push_back(vertex);
        angle += 360.0f / verticesCnt;
        vertex = {(disk.w / 2.0f) * std::cos(ToRadians(angle)), 
                  (disk.h / 2.0f) * std::sin(ToRadians(angle))};
    }
    Polygon<GLfloat> ret(vertices);

    return ret;
}

template<>
Polygon<GLfloat> Polygon<GLfloat>::CreateFromShape(
                                const Ring<GLfloat>& ring, int verticesCnt) {
    std::vector<Vector<GLfloat>> vertices;
    GLfloat angle = 0.0f;
    Vector<GLfloat> vertexInner = {ring.inner.w / 2.0f, 0.0f};
    Vector<GLfloat> vertexOuter = {ring.outer.w / 2.0f, 0.0f};
    for (int i = 1; i < verticesCnt; i++) {
        vertices.push_back(vertexInner);
        vertices.push_back(vertexOuter);
        angle += 360.0f / verticesCnt;
        vertexInner = {(ring.inner.w / 2.0f) * std::cos(ToRadians(angle)), 
                       (ring.inner.h / 2.0f) * std::sin(ToRadians(angle))};
        vertexOuter = {(ring.outer.w / 2.0f) * std::cos(ToRadians(angle)), 
                       (ring.outer.h / 2.0f) * std::sin(ToRadians(angle))};
    }
    Polygon<GLfloat> ret(vertices);

    return ret;
}

template<>
Polygon<GLfloat> Polygon<GLfloat>::CreateFromShape(const Rectangle<GLfloat>& rectangle) {
    std::vector<Vector<GLfloat>> vertices;
    vertices.push_back({-rectangle.w / 2.0f, -rectangle.h / 2.0f});
    vertices.push_back({-rectangle.w / 2.0f, rectangle.h / 2.0f});
    vertices.push_back({rectangle.w / 2.0f, rectangle.h / 2.0f});
    vertices.push_back({rectangle.w / 2.0f, -rectangle.h / 2.0f});
    Polygon<GLfloat> ret(vertices);

    return ret;
}

template<>
Polygon<GLfloat> Polygon<GLfloat>::CreateFromShape(
                                const Segment<GLfloat>& segment) {
    std::vector<Vector<GLfloat>> vertices; 
    Polygon<GLfloat> ret(vertices);

    return ret;
}

}
}
}
