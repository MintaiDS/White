#pragma once

#include "MeshManager.h"

namespace White {
namespace Engine {
namespace Graphics {

class Renderer {
public:
    Renderer(MeshManager& meshManager);

    void Render();
    void DrawCall(Mesh<GLfloat> mesh);

private:
    MeshManager& meshManager;
};

}
}
}
