#include "RenderData.h"
#include "ObjectManager.h"
#include "InterfaceProvider.h"
#include "Logger.h"

using namespace White::Util;

namespace White {
namespace Engine {
namespace Graphics {

RenderData::RenderData() : isInitialized(false) {}

void RenderData::Init() {
    vertexArray.Create();
    vertexArray.Bind();
    arrayBuffer.Create();
    arrayBuffer.Bind(GL_ARRAY_BUFFER);
    //textureBuffer.Create();
    //textureBuffer.Bind(GL_TEXTURE_BUFFER);
    elementArrayBuffer.Create();
    elementArrayBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER); 
}

void RenderData::Activate() {
    vertexArray.Bind();
    arrayBuffer.Bind(GL_ARRAY_BUFFER);
    //textureBuffer.Bind(GL_TEXTURE_BUFFER);
    elementArrayBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER); 
}

void RenderData::AddModel(unsigned model) {
    unusedModels.push_back(model);
}

void RenderData::SetModelFormat(ModelFormat modelFormat) {
    this->modelFormat = modelFormat;
}

void RenderData::Update() {
    int numComponents = 0;
    for (auto& it : modelFormat.numComponents) {
        numComponents += it;
    }

    auto& om = ObjectManager::GetInstance();
    InterfaceProvider ip;
    GLint summarySize = 0;
    GLint summaryIndicesCnt = 0;
    for (int i = 0; i < unusedModels.size(); i++) {
        auto modelObject = ip.Query<Model>(unusedModels[i]);
        auto mesh = ip.Query<Mesh<float>>(modelObject->GetMesh());
        models.push_back(unusedModels[i]);
        summarySize += mesh->GetSize();
        summaryIndicesCnt += mesh->indices.size();
    }
    int summaryCnt = summarySize / sizeof(GLfloat);
    
    GLvoid* oldDestDataPtr;    
    const GLvoid* oldSrcDataPtr;
    const GLvoid* newSrcDataPtr;
    const GLvoid* nullPtr;
    GLint prevSize = arrayBuffer.GetSize();
    GLint newSize = prevSize + summarySize;
    GLint prevCnt = prevSize / sizeof(GLfloat); 
    GLint newCnt = prevCnt + (newSize - prevSize) / sizeof(GLfloat);
    GLint prevArrayCnt = prevCnt / numComponents;
    std::unique_ptr<GLfloat[]> newArrayData
        = std::make_unique<GLfloat[]>(summaryCnt);
    std::unique_ptr<GLfloat[]> oldArrayData
        = std::make_unique<GLfloat[]>(prevCnt);
    oldDestDataPtr = reinterpret_cast<GLvoid*>(oldArrayData.get());
    oldSrcDataPtr = reinterpret_cast<const GLvoid*>(oldArrayData.get());
    newSrcDataPtr = reinterpret_cast<const GLvoid*>(newArrayData.get());
    nullPtr = reinterpret_cast<const GLvoid*>(nullptr);
    int curIndex = 0;
    for (int i = 0; i < unusedModels.size(); i++) {
        auto modelObject = ip.Query<Model>(unusedModels[i]);
        auto mesh = ip.Query<Mesh<float>>(modelObject->GetMesh());
        GLfloat* meshArrayData = mesh->GetRawData();
        int cnt = mesh->GetSize() / sizeof(GLfloat);
        for (int j = 0; j < cnt; j++) {
            newArrayData[curIndex] = meshArrayData[j]; 
            curIndex++;
        }
    }
    arrayBuffer.GetSubData(0, prevSize, oldDestDataPtr);
    arrayBuffer.SetData(newSize, nullPtr, GL_DYNAMIC_DRAW);
    arrayBuffer.SetSubData(0, prevSize, oldSrcDataPtr);
    arrayBuffer.SetSubData(prevSize, summarySize, newSrcDataPtr);
    
    prevSize = elementArrayBuffer.GetSize();
    newSize = prevSize + sizeof(GLuint) * summaryIndicesCnt;
    prevCnt = prevSize / sizeof(GLuint);
    newCnt = prevCnt + (newSize - prevSize) / sizeof(GLuint);
    std::unique_ptr<GLuint[]> newElementArrayData
        = std::make_unique<GLuint[]>(summaryIndicesCnt);
    std::unique_ptr<GLuint[]> oldElementArrayData 
        = std::make_unique<GLuint[]>(prevCnt);
    oldDestDataPtr = reinterpret_cast<GLvoid*>(oldElementArrayData.get());
    oldSrcDataPtr = reinterpret_cast<const GLvoid*>(oldElementArrayData.get());
    newSrcDataPtr = reinterpret_cast<const GLvoid*>(newElementArrayData.get()); 
    GLint curVerticesCnt = 0;
    GLint size = summaryIndicesCnt * sizeof(GLuint); 
    GLint curSize = 0;
    curIndex = 0;
    for (int i = 0; i < unusedModels.size(); i++) {
        auto modelObject = ip.Query<Model>(unusedModels[i]);
        auto mesh = ip.Query<Mesh<float>>(modelObject->GetMesh());
        GLuint* meshElementArrayData = mesh->GetRawIndices();
        for (int j = 0; j < mesh->indices.size(); j++) {
            newElementArrayData[curIndex] = meshElementArrayData[j];
            newElementArrayData[curIndex] += prevArrayCnt + curVerticesCnt;
            curIndex++;
        }
        curSize += mesh->indices.size() * sizeof(GLuint);
        curVerticesCnt += mesh->GetSize() / sizeof(GLfloat) / numComponents;
    }
    elementArrayBuffer.GetSubData(0, prevSize, oldDestDataPtr);
    elementArrayBuffer.SetData(newSize, nullPtr, GL_DYNAMIC_DRAW); 
    elementArrayBuffer.SetSubData(0, prevSize, oldSrcDataPtr);
    elementArrayBuffer.SetSubData(prevSize, size, newSrcDataPtr);
    std::vector<const void*> offsetPtrs;
    auto blockSize = 0;
    for (auto& it : modelFormat.numComponents) {
        const void* ptr = reinterpret_cast<const void*>(blockSize);
        blockSize += sizeof(float) * it; 
        offsetPtrs.push_back(ptr);
    }
    for (int i = 0; i < modelFormat.numAttributes; i++) {
        glVertexAttribPointer(i, modelFormat.numComponents[i], 
                              GL_FLOAT, GL_FALSE, blockSize, offsetPtrs[i]);
        glEnableVertexAttribArray(i);
    }

    unusedModels.clear();
}

void RenderData::UpdateData(unsigned model) {
    if (modelFormat.isTextured) {
        InterfaceProvider ip;
        auto modelObject = ip.Query<Model>(model);
        Texture& texture = modelObject->GetTexture();
        texture.Bind(); 
    }
}
    
std::vector<unsigned>& RenderData::GetModels() {
    return models;
}

}
}
}
