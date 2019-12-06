#include "GLFunctions.h"
#include "Renderer.h"
#include "Disk.h"
#include "Ring.h"
#include "Rectangle.h"
#include "Game.h"
#include "GraphVisualizer.h"
#include "ObjectManager.h"
#include "InterfaceProvider.h"
#include "Logger.h"

#include <fstream>
#include <iostream>

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Graphics {

Renderer::Renderer() {
    lastTime = std::chrono::duration_cast<std::chrono::milliseconds>
               (std::chrono::system_clock::now().time_since_epoch());
}

void Renderer::Init() {
    game = new GraphVisualizer(*this);
}

void Renderer::Render() {
    std::chrono::milliseconds cur 
        = std::chrono::duration_cast<std::chrono::milliseconds>
          (std::chrono::system_clock::now().time_since_epoch());
    std::chrono::milliseconds time = cur - lastTime;
    //if (time > std::chrono::milliseconds{24}) {
        game->Play();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
        for (int i = 0; i < contextStates.size(); i++) {
            contextStates[i].Activate();
            contextStates[i].UpdateCamera(view, projection);
            contextStates[i].Render();
        }
        lastTime = cur;
    //}
}

void Renderer::SetView(Matrix<float> view) {
    this->view = view;
}

void Renderer::SetProjection(Matrix<float> projection) {
    this->projection = projection;
}


void Renderer::DrawCall(int indicesCnt, int skip) {
}

void Renderer::UpdateData(unsigned mesh) { 
}

void Renderer::AddMesh(unsigned mesh) {
}

void Renderer::AddModel(unsigned model) {
    InterfaceProvider ip;
    auto modelInstance = *ip.Query<Model>(model);
    ModelFormat fmt = modelInstance.GetFormat();
    for (auto& it : contextStates) {
        if (it.GetModelFormat() == fmt) {
            it.GetRenderData().AddModel(model);
            
            return;
        }
    }
    ContextState contextState;
    RenderData renderData;
    renderData.AddModel(model);
    contextState.SetModelFormat(fmt);
    contextState.SetRenderData(renderData);
    contextStates.push_back(contextState); 
}

void Renderer::UpdateVertexData() {
    for (auto& it : contextStates) {
        if (!it.IsInitialized()) {
            it.Init();
        }        
    }
}

Program& Renderer::GetProgram() {
    return program;
}

}
}
}
