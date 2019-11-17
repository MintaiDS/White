#include "Rectangle.h"
#include "Polygon.h"

namespace White {
namespace Util {
namespace Math {

template<>
Util::Math::Polygon<GLfloat> Rectangle<GLfloat>::ToPolygon(int verticesCnt) {
    std::vector<Vector<GLfloat>> vertices;
    vertices.push_back({-w / 2.0f, -h / 2.0f});
    vertices.push_back({-w / 2.0f, h / 2.0f});
    vertices.push_back({w / 2.0f, h / 2.0f});
    vertices.push_back({w / 2.0f, -h / 2.0f});
    Util::Math::Polygon<GLfloat> ret(vertices);

    return ret;
}

}
}
}
