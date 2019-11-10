#include "Renderer.h"

namespace White {
namespace Engine {
namespace Graphics {

void Renderer::Render(MeshManager& meshManager) {
    for (auto mesh : meshManager.list) {
        DrawCall(mesh);
    }
}

void Renderer::DrawCall(Mesh<GLfloat> mesh) {
    
}

}
}
}


