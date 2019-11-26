#pragma once

#include "RenderData.h"
#include "Program.h"
#include "Shader.h"

namespace White {
namespace Engine {
namespace Graphics {

class ContextState {
public:
    ContextState();

    void Init();
    void Activate();
    void Destroy();
    void SetRenderData(RenderData renderData);
    RenderData& GetRenderData();
    ModelFormat GetModelFormat();

protected:
    Program program;
    Shader vertexShader;
    Shader fragmentShader;
    RenderData renderData;
    ModelFormat modelFormat;
};

}
}
}
