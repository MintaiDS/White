#include "Ring.h"

namespace White {
namespace Util {
namespace Math {

template<>
Polygon<GLfloat> Ring<GLfloat>::ToPolygon(int verticesCnt) {
    std::vector<Vector<GLfloat>> vertices;
    GLfloat angle = 0.0f;
    Vector<GLfloat> vertexInner = {inner.w / 2.0f, 0.0f};
    Vector<GLfloat> vertexOuter = {outer.w / 2.0f, 0.0f};
    for (int i = 1; i < verticesCnt; i++) {
        vertices.push_back(vertexInner);
        vertices.push_back(vertexOuter);
        angle += 360.0f / verticesCnt;
        vertexInner = {(inner.w / 2.0f) * std::cos(ToRadians(angle)), 
                       (inner.h / 2.0f) * std::sin(ToRadians(angle))};
        vertexOuter = {(outer.w / 2.0f) * std::cos(ToRadians(angle)), 
                       (outer.h / 2.0f) * std::sin(ToRadians(angle))};
    }
    Polygon<GLfloat> ret(vertices);

    return ret;
}

}
}
}

