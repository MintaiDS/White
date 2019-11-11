#pragma once

#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "Mesh.h"

namespace White {
namespace Engine {
namespace Graphics {

class Renderer {
public:
    Renderer();

    void Render();
    void DrawCall(int indicesCnt = 0, int skip = 0);
    void UpdateData(Mesh<GLfloat> mesh);
    void AddMesh(Mesh<GLfloat> mesh);

protected:
    std::vector<Mesh<GLfloat>> list;
    int indicesCnt;
    BufferObject arrayBuffer;
    BufferObject elementArrayBuffer;
    VertexArrayObject vertexArray;
};

}
}
}
