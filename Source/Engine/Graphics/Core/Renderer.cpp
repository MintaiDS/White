#include "Renderer.h"

namespace White {
namespace Engine {
namespace Graphics {

Renderer::Renderer(MeshManager& meshManager) : meshManager(meshManager) {}

void Renderer::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    for (int i = 0; i < meshManager.list.size(); i++) {
        DrawCall(meshManager.list[i]);
    }
}

void Renderer::DrawCall(Mesh<GLfloat> mesh) {
    glDrawElements(GL_TRIANGLES, mesh.indices.size(),
                   GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(nullptr));
}

}
}
}
