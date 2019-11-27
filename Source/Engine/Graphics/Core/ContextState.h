#pragma once

#include "DrawingCommand.h"
#include "RenderData.h"
#include "Program.h"
#include "Shader.h"

namespace White {
namespace Engine {
namespace Graphics {

class ContextState {
public:
    ContextState();

    bool IsInitialized() const;
    void Init();
    void Activate();
    void Destroy();
    void Render();
    void SetRenderData(RenderData renderData);
    void SetModelFormat(ModelFormat modelFormat);
    RenderData& GetRenderData();
    ModelFormat GetModelFormat();

protected:
    bool isInitialized;
    Program program;
    Shader vertexShader;
    Shader fragmentShader;
    RenderData renderData;
    ModelFormat modelFormat;
    DrawingCommand drawingCommand;
};

}
}
}
