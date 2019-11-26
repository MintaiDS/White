#include "ContextState.h"

namespace White {
namespace Engine {
namespace Graphics {

ContextState::ContextState() {}

void ContextState::Init() {
    program.Create();
    vertexShader.Create(GL_VERTEX_SHADER);
    std::wstring path = modelFormat.shaders[0];
    vertexShader.Source(path);
    vertexShader.Compile();
    program.Attach(vertexShader);
    path = modelFormat.shaders[1];
    fragmentShader.Create(GL_FRAGMENT_SHADER);
    fragmentShader.Source(path);
    fragmentShader.Compile();
    program.Attach(fragmentShader);
    program.Link();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);  
}

void ContextState::Activate() {
    program.Use();
}

void ContextState::Destroy() {
    program.Detach(vertexShader);
    program.Detach(fragmentShader);
    vertexShader.Delete();
    fragmentShader.Delete();
    program.Delete();
}

void ContextState::SetRenderData(RenderData renderData) {
    this->renderData = renderData;
}

RenderData& ContextState::GetRenderData() {
    return renderData;
}

ModelFormat ContextState::GetModelFormat() {
    return modelFormat;
}

}
}
}
