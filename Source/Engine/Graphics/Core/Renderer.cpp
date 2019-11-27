#include "GLFunctions.h"
#include "Renderer.h"
#include "Disk.h"
#include "Ring.h"
#include "Rectangle.h"
#include "Game.h"
#include "GraphVisualizer.h"
#include "ObjectManager.h"
#include "InterfaceProvider.h"

#include <fstream>
#include <iostream>

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Graphics {

Renderer::Renderer() {
}

void Renderer::Init() {
    game = new GraphVisualizer(*this);
}

void Renderer::Render() {
    ObjectManager& om = ObjectManager::GetInstance();
    InterfaceProvider ip; 
    game->Play();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.90f, 0.90f, 0.90f, 1.0f);
    for (int i = 0; i < contextStates.size(); i++) {
        contextStates[i].Activate();
        contextStates[i].Render();
    }
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
