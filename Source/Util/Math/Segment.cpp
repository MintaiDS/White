#include "Segment.h"
#include "Rectangle.h"

namespace White {
namespace Util {
namespace Math {

template<>
Polygon<GLfloat> Segment<GLfloat>::ToPolygon(int verticesCnt) {
    GLfloat len = (this->end - this->begin).Length();
    Rectangle<GLfloat> rect(len, 0.001f);

    return rect.ToPolygon(verticesCnt);
}

}
}
}
