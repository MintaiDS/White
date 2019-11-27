#pragma once

#include "Model.h"
#include "BufferObject.h"
#include "VertexArrayObject.h"

namespace White {
namespace Engine {
namespace Graphics {

class RenderData {
public:
    RenderData();

    void Init();
    void Activate();
    void AddModel(unsigned model);
    void Update();
    std::vector<unsigned>& GetModels();
    void SetModelFormat(ModelFormat modelFormat);

protected:
    bool isInitialized;
    std::vector<unsigned> models;
    std::vector<unsigned> unusedModels;
    BufferObject arrayBuffer;
    BufferObject textureBuffer;
    BufferObject elementArrayBuffer;
    VertexArrayObject vertexArray;
    ModelFormat modelFormat;
};

}
}
}
