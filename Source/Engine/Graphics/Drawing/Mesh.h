#pragma once

#include "Vector.h"

#include <windows.h>
#include <gl/gl.h>

namespace White {
namespace Engine {
namespace Render {

struct Mesh {
    Mesh(const Mesh& other);
    Mesh(const std::vector<Util::Math::Vector<GLfloat>>& vertices);
    Mesh(const std::size_t size, const Util::Math::Vector<GLfloat>* vertices);

    std::vector<Util::Math::Vector<GLfloat>> vertices;
};

}
}
}
