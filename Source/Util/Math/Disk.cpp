#include "Disk.h"

namespace White {
namespace Util {
namespace Math {

template<>
Polygon<GLfloat> Disk<GLfloat>::ToPolygon(int verticesCnt) {
    std::vector<Vector<GLfloat>> vertices;
    GLfloat angle = 0.0f;
    Vector<GLfloat> vertex = {w / 2.0f, 0.0f};
    for (int i = 0; i < verticesCnt; i++) {
        vertices.push_back(vertex);
        angle += (360.0f + 10.0f) / verticesCnt;
        vertex = {(w / 2.0f) * std::cos(ToRadians(angle)), 
                  (h / 2.0f) * std::sin(ToRadians(angle))};
    }
    Polygon<GLfloat> ret(vertices);

    return ret;
}

}
}
}
