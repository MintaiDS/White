#include "Polygon.h"

namespace White {
namespace Util {
namespace Math {

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
