#pragma once

#include "Object.h"
#include "MeshManager.h"

namespace White {
namespace Engine {
namespace Graphics {

class Renderer {
public:
    void Render(MeshManager& meshManager);
    void DrawCall(Mesh<GLfloat> mesh);
};

}
}
}
