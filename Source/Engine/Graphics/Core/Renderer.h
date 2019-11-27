#pragma once

#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "Program.h"
#include "Mesh.h"
#include "Model.h"
#include "ContextState.h"

namespace White {
namespace Engine {
namespace Graphics {

class GraphVisualizer;
class Game;

class Renderer {
public:
    Renderer();
    
    void Init();
    void Render();
    void DrawCall(int indicesCnt = 0, int skip = 0);
    void UpdateData(unsigned mesh);
    void AddMesh(unsigned mesh); 
    void AddModel(unsigned model);
    void UpdateVertexData();
    Program& GetProgram();

protected:
    Program program; 
    GraphVisualizer* game;
    std::vector<ContextState> contextStates;
    std::vector<unsigned> models;
    std::vector<unsigned> unusedModels;
    std::vector<unsigned> list;
    std::vector<unsigned> unused;
    int indicesCnt;
    BufferObject arrayBuffer;
    BufferObject elementArrayBuffer;
    VertexArrayObject vertexArray;
};

}
}
}
