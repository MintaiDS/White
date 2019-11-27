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
    vertexArray.Create();
    vertexArray.Bind();
    arrayBuffer.Create();
    arrayBuffer.Bind(GL_ARRAY_BUFFER);
    elementArrayBuffer.Create();
    elementArrayBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER); 
}

void Renderer::Init() {
    //program.Create();
    //Shader shader;
    //shader.Create(GL_VERTEX_SHADER);
    //std::wstring path = L"Engine/Shaders/default.vsh";
    //shader.Source(path);
    //shader.Compile();
    //program.Attach(shader);
    //shader.Delete();
    //path = L"Engine/Shaders/default.fsh";
    //shader.Create(GL_FRAGMENT_SHADER);
    //shader.Source(path);
    //shader.Compile();
    //program.Attach(shader);
    //shader.Delete();
    //program.Link();
    //program.Use();
    //program.Delete();
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CCW); 
    //Matrix<GLfloat> view = Matrix<GLfloat>::Identity(4);
    //std::unique_ptr<GLfloat[]> raw 
    //    = std::make_unique<GLfloat[]>(view.rows * view.columns);
    //GLint location = glGetUniformLocation(program.id, "view");
    //for (int i = 0; i < view.rows; i++) {
    //    for (int j = 0; j < view.columns; j++) {
    //        raw.get()[i * view.columns + j] = view[i][j];
    //    }
    //}
    //glProgramUniformMatrix4fv(program.id, location, 1, GL_TRUE, raw.get());
    game = new GraphVisualizer(*this);
}

void Renderer::Render() {
    ObjectManager& om = ObjectManager::GetInstance();
    InterfaceProvider ip; 
    game->Play();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    for (int i = 0; i < contextStates; i++) {
        contextStates[i].Activate();
        contextStates[i].Render();
    }
    //indicesCnt = 0;
    //for (int i = 0; i < list.size(); i++) {
    //    auto mesh = *ip.Query<Mesh<float>>(list[i]);

    //    //Mesh<GLfloat>& mesh = list[i]; 
    //    Vector<GLfloat> rotation = mesh.GetRotation();
    //    Matrix<GLfloat> rotationMatrix 
    //        = Matrix<GLfloat>::Rotation({rotation[0], rotation[1], 
    //                                     rotation[2], 1.0f});  
    //    Vector<GLfloat> translation = mesh.GetTranslation();
    //    Matrix<GLfloat> translationMatrix
    //        = Matrix<GLfloat>::Translation(translation);
    //    Vector<GLfloat> scaling = mesh.GetScaling();
    //    Matrix<GLfloat> scalingMatrix 
    //        = Matrix<GLfloat>::Scaling({scaling[0], scaling[1], 
    //                                    scaling[2], 1.0f});
    //    Matrix<GLfloat> model = translationMatrix 
    //                            * rotationMatrix 
    //                            * scalingMatrix;
    //    std::unique_ptr<GLfloat[]> raw 
    //        = std::make_unique<GLfloat[]>(model.rows * model.columns);
    //    for (int i = 0; i < model.rows; i++) {
    //        for (int j = 0; j < model.columns; j++) {
    //            raw.get()[i * model.columns + j] = model[i][j];
    //        }
    //    }
    //    GLint location = glGetUniformLocation(program.id, "model");
    //    glProgramUniformMatrix4fv(program.id, location, 1, GL_TRUE, raw.get());
    //    DrawCall(mesh.indices.size(), indicesCnt);
    //    indicesCnt += mesh.indices.size();
    //}
}

void Renderer::DrawCall(int indicesCnt, int skip) {
    glDrawElements(GL_TRIANGLES, 
                   indicesCnt, 
                   GL_UNSIGNED_INT, 
                   reinterpret_cast<const GLvoid*>(skip * sizeof(GLuint))); 
}

void Renderer::UpdateData(unsigned mesh) { 
    //ObjectManager& om = ObjectManager::GetInstance();
    //InterfaceProvider ip;
    //auto meshPtr = *ip.Query<Mesh<float>>(mesh);

    //GLint prevSize = arrayBuffer.GetSize();
    //GLint newSize = prevSize + meshPtr.GetSize();
    //GLint prevCnt = prevSize / sizeof(GLfloat); 
    //GLint newCnt = prevCnt + (newSize - prevSize) / sizeof(GLfloat);
    //GLfloat* newArrayData = meshPtr.GetRawData();
    //std::unique_ptr<GLfloat[]> oldArrayData
    //    = std::make_unique<GLfloat[]>(prevCnt);
    //arrayBuffer.GetSubData(0, prevSize, 
    //                       reinterpret_cast<GLvoid*>(oldArrayData.get()));
    //arrayBuffer.SetData(newSize, 
    //                reinterpret_cast<const GLvoid*>(nullptr), GL_DYNAMIC_DRAW);
    //arrayBuffer.SetSubData(0, prevSize, 
    //                    reinterpret_cast<const GLvoid*>(oldArrayData.get()));
    //arrayBuffer.SetSubData(prevSize, (newSize - prevSize), 
    //                       reinterpret_cast<const GLvoid*>(newArrayData));
    //GLint prevArrayCnt = prevCnt / 8;
    //
    //prevSize = elementArrayBuffer.GetSize();
    //newSize = prevSize + sizeof(GLuint) * meshPtr.indices.size();
    //prevCnt = prevSize / sizeof(GLuint);
    //newCnt = prevCnt + (newSize - prevSize) / sizeof(GLuint);
    //GLuint* newElementArrayData = meshPtr.GetRawIndices();
    //for (int i = 0; i < meshPtr.indices.size(); i++) {
    //    newElementArrayData[i] += prevArrayCnt;
    //}
    //std::unique_ptr<GLuint[]> oldElementArrayData 
    //    = std::make_unique<GLuint[]>(prevCnt);
    //elementArrayBuffer.GetSubData(0, prevSize, 
    //                reinterpret_cast<GLvoid*>(oldElementArrayData.get()));
    //elementArrayBuffer.SetData(newSize, 
    //            reinterpret_cast<const GLvoid*>(nullptr), GL_DYNAMIC_DRAW);
    //elementArrayBuffer.SetSubData(0, prevSize,
    //            reinterpret_cast<const GLvoid*>(oldElementArrayData.get()));
    //elementArrayBuffer.SetSubData(prevSize, (newSize - prevSize),
    //                reinterpret_cast<const GLvoid*>(newElementArrayData));

    //glVertexAttribPointer(0, 4, 
    //                      GL_FLOAT, GL_FALSE, 
    //                      sizeof(GLfloat) * 4 * 2, 
    //                      reinterpret_cast<const GLvoid*>(0));
    //glVertexAttribPointer(1, 4, 
    //                    GL_FLOAT, GL_FALSE, 
    //                    sizeof(GLfloat) * 4 * 2, 
    //                    reinterpret_cast<const GLvoid*>(sizeof(GLfloat) * 4));
    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
}

void Renderer::AddMesh(unsigned mesh) {
    //unused.push_back(mesh);
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
    contextStates.push_back(contextState); 
}

void Renderer::UpdateVertexData() {
    for (auto& it : contextStates) {
        if (!it.IsInitialized()) {
            it.Init();
        }
        
    }
    //auto& om = ObjectManager::GetInstance();
    //InterfaceProvider ip;
    //GLint summarySize = 0;
    //GLint summaryIndicesCnt = 0;
    //for (int i = 0; i < unused.size(); i++) {
    //    //auto meshId = om.Create<Mesh<float>>(unused[i]);
    //    //auto mesh = om.GetObjectById(meshId);
    //    //InterfaceProvider ip;
    //    auto mesh = *ip.Query<Mesh<float>>(unused[i]);
    //    list.push_back(unused[i]);
    //    summarySize += mesh.GetSize();
    //    summaryIndicesCnt += mesh.indices.size();
    //}
    //int summaryCnt = summarySize / sizeof(GLfloat);
    //
    //GLvoid* oldDestDataPtr;    
    //const GLvoid* oldSrcDataPtr;
    //const GLvoid* newSrcDataPtr;
    //const GLvoid* nullPtr;
    //GLint prevSize = arrayBuffer.GetSize();
    //GLint newSize = prevSize + summarySize;
    //GLint prevCnt = prevSize / sizeof(GLfloat); 
    //GLint newCnt = prevCnt + (newSize - prevSize) / sizeof(GLfloat);
    //GLint prevArrayCnt = prevCnt / 8;
    //std::unique_ptr<GLfloat[]> newArrayData
    //    = std::make_unique<GLfloat[]>(summaryCnt);
    //std::unique_ptr<GLfloat[]> oldArrayData
    //    = std::make_unique<GLfloat[]>(prevCnt);
    //oldDestDataPtr = reinterpret_cast<GLvoid*>(oldArrayData.get());
    //oldSrcDataPtr = reinterpret_cast<const GLvoid*>(oldArrayData.get());
    //newSrcDataPtr = reinterpret_cast<const GLvoid*>(newArrayData.get());
    //nullPtr = reinterpret_cast<const GLvoid*>(nullptr);
    //int curIndex = 0;
    //for (int i = 0; i < unused.size(); i++) {
    //    auto mesh = *ip.Query<Mesh<float>>(unused[i]);
    //    GLfloat* meshArrayData = mesh.GetRawData();
    //    int cnt = mesh.GetSize() / sizeof(GLfloat);
    //    for (int j = 0; j < cnt; j++) {
    //        newArrayData[curIndex] = meshArrayData[j]; 
    //        curIndex++;
    //    }
    //}
    //arrayBuffer.GetSubData(0, prevSize, oldDestDataPtr);
    //arrayBuffer.SetData(newSize, nullPtr, GL_DYNAMIC_DRAW);
    //arrayBuffer.SetSubData(0, prevSize, oldSrcDataPtr);
    //arrayBuffer.SetSubData(prevSize, summarySize, newSrcDataPtr);
    //
    //prevSize = elementArrayBuffer.GetSize();
    //newSize = prevSize + sizeof(GLuint) * summaryIndicesCnt;
    //prevCnt = prevSize / sizeof(GLuint);
    //newCnt = prevCnt + (newSize - prevSize) / sizeof(GLuint);
    //std::unique_ptr<GLuint[]> newElementArrayData
    //    = std::make_unique<GLuint[]>(summaryIndicesCnt);
    //std::unique_ptr<GLuint[]> oldElementArrayData 
    //    = std::make_unique<GLuint[]>(prevCnt);
    //oldDestDataPtr = reinterpret_cast<GLvoid*>(oldElementArrayData.get());
    //oldSrcDataPtr = reinterpret_cast<const GLvoid*>(oldElementArrayData.get());
    //newSrcDataPtr = reinterpret_cast<const GLvoid*>(newElementArrayData.get()); 
    //GLint curVerticesCnt = 0;
    //GLint size = summaryIndicesCnt * sizeof(GLuint); 
    //GLint curSize = 0;
    //curIndex = 0;
    //for (int i = 0; i < unused.size(); i++) {
    //    auto mesh = *ip.Query<Mesh<float>>(unused[i]);
    //    GLuint* meshElementArrayData = mesh.GetRawIndices();
    //    for (int j = 0; j < mesh.indices.size(); j++) {
    //        newElementArrayData[curIndex] = meshElementArrayData[j];
    //        newElementArrayData[curIndex] += prevArrayCnt + curVerticesCnt;
    //        curIndex++;
    //    }
    //    curSize += mesh.indices.size() * sizeof(GLuint);
    //    curVerticesCnt += mesh.GetSize() / sizeof(GLfloat) / 8;
    //}
    //elementArrayBuffer.GetSubData(0, prevSize, oldDestDataPtr);
    //elementArrayBuffer.SetData(newSize, nullPtr, GL_DYNAMIC_DRAW); 
    //elementArrayBuffer.SetSubData(0, prevSize, oldSrcDataPtr);
    //elementArrayBuffer.SetSubData(prevSize, size, newSrcDataPtr);
    //            
    //GLint blockSize = sizeof(GLfloat) * 4 * 2;
    //const GLvoid* offsetPtrs[] = {
    //    reinterpret_cast<const GLvoid*>(0),
    //    reinterpret_cast<const GLvoid*>(sizeof(GLfloat) * 4)
    //};
    //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, blockSize, offsetPtrs[0]);
    //glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, blockSize, offsetPtrs[1]);
    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);

    //unused.clear();
}

Program& Renderer::GetProgram() {
    return program;
}

}
}
}
