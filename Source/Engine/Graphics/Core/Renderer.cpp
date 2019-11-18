#include "GLFunctions.h"
#include "Renderer.h"
#include "Disk.h"
#include "Ring.h"
#include "Rectangle.h"
#include "Game.h"
#include "GraphVisualizer.h"

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
    program.Create();
    Shader shader(GL_VERTEX_SHADER);
    std::wstring path = L"Engine/Shaders/default.vsh";
    shader.Source(path);
    shader.Compile();
    program.Attach(shader);
    shader.Delete();
    path = L"Engine/Shaders/default.fsh";
    shader.Create(GL_FRAGMENT_SHADER);
    shader.Source(path);
    shader.Compile();
    program.Attach(shader);
    shader.Delete();
    program.Link();
    program.Use();
    //program.Delete();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW); 
    Matrix<GLfloat> view = Matrix<GLfloat>::Identity(4);
    std::unique_ptr<GLfloat[]> raw 
        = std::make_unique<GLfloat[]>(view.rows * view.columns);
    program.Use();
    GLint location = glGetUniformLocation(program.id, "view");
    for (int i = 0; i < view.rows; i++) {
        for (int j = 0; j < view.columns; j++) {
            raw.get()[i * view.columns + j] = view[i][j];
        }
    }
    program.Use();
    glProgramUniformMatrix4fv(program.id, location, 1, GL_TRUE, raw.get());
    game = new GraphVisualizer(*this);
}

void Renderer::Render() {
    game->Play();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    indicesCnt = 0;
    for (int i = 0; i < list.size(); i++) {
        Mesh<GLfloat>& mesh = list[i]; 
        Vector<GLfloat> rotation = mesh.GetRotation();
        Matrix<GLfloat> rotationMatrix 
            = Matrix<GLfloat>::Rotation({rotation[0], rotation[1], 
                                         rotation[2], 1.0f});  
        Vector<GLfloat> translation = mesh.GetTranslation();
        Matrix<GLfloat> translationMatrix
            = Matrix<GLfloat>::Translation(translation);
        Vector<GLfloat> scaling = mesh.GetScaling();
        Matrix<GLfloat> scalingMatrix 
            = Matrix<GLfloat>::Scaling({scaling[0], scaling[1], 
                                        scaling[2], 1.0f});
        Matrix<GLfloat> model = translationMatrix 
                                * rotationMatrix 
                                * scalingMatrix;
        std::unique_ptr<GLfloat[]> raw 
            = std::make_unique<GLfloat[]>(model.rows * model.columns);
        for (int i = 0; i < model.rows; i++) {
            for (int j = 0; j < model.columns; j++) {
                raw.get()[i * model.columns + j] = model[i][j];
            }
        }
        program.Use();
        GLint location = glGetUniformLocation(program.id, "model");
        program.Use();
        glProgramUniformMatrix4fv(program.id, location, 1, GL_TRUE, raw.get());
        DrawCall(list[i].indices.size(), indicesCnt);
        indicesCnt += list[i].indices.size();
    }
    //DrawCall(indicesCnt);
}

void Renderer::DrawCall(int indicesCnt, int skip) {
    glDrawElements(GL_TRIANGLES, 
                   indicesCnt, 
                   GL_UNSIGNED_INT, 
                   reinterpret_cast<const GLvoid*>(skip * sizeof(GLuint))); 
}

void Renderer::UpdateData(Mesh<GLfloat> mesh) { 
    GLint prevSize = arrayBuffer.GetSize();
    GLint newSize = prevSize + mesh.GetSize();
    GLint prevCnt = prevSize / sizeof(GLfloat); 
    GLint newCnt = prevCnt + (newSize - prevSize) / sizeof(GLfloat);
    GLfloat* newArrayData = mesh.GetRawData();
    std::unique_ptr<GLfloat[]> oldArrayData
        = std::make_unique<GLfloat[]>(prevCnt);
    arrayBuffer.GetSubData(0, prevSize, 
                           reinterpret_cast<GLvoid*>(oldArrayData.get()));
    arrayBuffer.SetData(newSize, 
                    reinterpret_cast<const GLvoid*>(nullptr), GL_DYNAMIC_DRAW);
    arrayBuffer.SetSubData(0, prevSize, 
                        reinterpret_cast<const GLvoid*>(oldArrayData.get()));
    arrayBuffer.SetSubData(prevSize, (newSize - prevSize), 
                           reinterpret_cast<const GLvoid*>(newArrayData));
    GLint prevArrayCnt = prevCnt / 8;
    
    prevSize = elementArrayBuffer.GetSize();
    newSize = prevSize + sizeof(GLuint) * mesh.indices.size();
    prevCnt = prevSize / sizeof(GLuint);
    newCnt = prevCnt + (newSize - prevSize) / sizeof(GLuint);
    GLuint* newElementArrayData = mesh.GetRawIndices();
    for (int i = 0; i < mesh.indices.size(); i++) {
        newElementArrayData[i] += prevArrayCnt;
    }
    //GLuint* oldElementArrayData = new GLuint[prevCnt];
    std::unique_ptr<GLuint[]> oldElementArrayData 
        = std::make_unique<GLuint[]>(prevCnt);
    elementArrayBuffer.GetSubData(0, prevSize, 
                    reinterpret_cast<GLvoid*>(oldElementArrayData.get()));
    elementArrayBuffer.SetData(newSize, 
                reinterpret_cast<const GLvoid*>(nullptr), GL_DYNAMIC_DRAW);
    elementArrayBuffer.SetSubData(0, prevSize,
                reinterpret_cast<const GLvoid*>(oldElementArrayData.get()));
    elementArrayBuffer.SetSubData(prevSize, (newSize - prevSize),
                    reinterpret_cast<const GLvoid*>(newElementArrayData));

    glVertexAttribPointer(0, 4, 
                          GL_FLOAT, GL_FALSE, 
                          sizeof(GLfloat) * 4 * 2, 
                          reinterpret_cast<const GLvoid*>(0));
    glVertexAttribPointer(1, 4, 
                        GL_FLOAT, GL_FALSE, 
                        sizeof(GLfloat) * 4 * 2, 
                        reinterpret_cast<const GLvoid*>(sizeof(GLfloat) * 4));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void Renderer::AddMesh(Mesh<GLfloat> mesh) {
    //list.push_back(mesh);
    //UpdateData(mesh);
    unused.push_back(mesh);
}

void Renderer::UpdateVertexData() {
    GLint summarySize = 0;
    GLint summaryIndicesCnt = 0;
    for (int i = 0; i < unused.size(); i++) {
        list.push_back(unused[i]);
        summarySize += unused[i].GetSize();
        summaryIndicesCnt += unused[i].indices.size();
    }
    int summaryCnt = summarySize / sizeof(GLfloat);
    
    GLint prevSize = arrayBuffer.GetSize();
    GLint newSize = prevSize + summarySize;
    GLint prevCnt = prevSize / sizeof(GLfloat); 
    GLint newCnt = prevCnt + (newSize - prevSize) / sizeof(GLfloat);

    std::unique_ptr<GLfloat[]> newArrayData
        = std::make_unique<GLfloat[]>(summaryCnt);
    std::unique_ptr<GLfloat[]> oldArrayData
        = std::make_unique<GLfloat[]>(prevCnt);
    GLvoid* destDataPtr 
        = reinterpret_cast<GLvoid*>(oldArrayData.get());
    const GLvoid* srcDataPtr 
        = reinterpret_cast<const GLvoid*>(oldArrayData.get());
    const GLvoid* nullPtr 
        = reinterpret_cast<const GLvoid*>(nullptr);
    arrayBuffer.GetSubData(0, prevSize, destDataPtr);
    arrayBuffer.SetData(newSize, nullPtr, GL_DYNAMIC_DRAW);
    arrayBuffer.SetSubData(0, prevSize, srcDataPtr);

    GLint curSize = 0;
    int curIndex = 0;
    for (int i = 0; i < unused.size(); i++) {
        GLfloat* meshArrayData = unused[i].GetRawData();
        int cnt = unused[i].GetSize() / sizeof(GLfloat);
        for (int i = 0; i < cnt; i++) {
            newArrayData[curIndex] = meshArrayData[i]; 
            curIndex++;
        }
    }
    srcDataPtr = reinterpret_cast<const GLvoid*>(newArrayData.get());
    arrayBuffer.SetSubData(prevSize, summarySize, srcDataPtr);
    GLint prevArrayCnt = prevCnt / 8;
    
    prevSize = elementArrayBuffer.GetSize();
    newSize = prevSize + sizeof(GLuint) * summaryIndicesCnt;
    prevCnt = prevSize / sizeof(GLuint);
    newCnt = prevCnt + (newSize - prevSize) / sizeof(GLuint);
    std::unique_ptr<GLuint[]> newElementArrayData
        = std::make_unique<GLuint[]>(summaryIndicesCnt);
    std::unique_ptr<GLuint[]> oldElementArrayData 
        = std::make_unique<GLuint[]>(prevCnt);
    destDataPtr = reinterpret_cast<GLvoid*>(oldElementArrayData.get());
    srcDataPtr = reinterpret_cast<const GLvoid*>(oldElementArrayData.get());
    elementArrayBuffer.GetSubData(0, prevSize, destDataPtr);
    elementArrayBuffer.SetData(newSize, nullPtr, GL_DYNAMIC_DRAW); 
    elementArrayBuffer.SetSubData(0, prevSize, srcDataPtr);
    GLint curVerticesCnt = 0;
    curSize = 0;
    curIndex = 0;
    for (int i = 0; i < unused.size(); i++) {
        GLuint* meshElementArrayData = unused[i].GetRawIndices();
        for (int j = 0; j < unused[i].indices.size(); j++) {
            newElementArrayData[curIndex] = meshElementArrayData[j];
            newElementArrayData[curIndex] += prevArrayCnt + curVerticesCnt;
            curIndex++;
        }
        curSize += unused[i].indices.size() * sizeof(GLuint);
        curVerticesCnt += unused[i].GetSize() / sizeof(GLfloat) / 8;
    }
    srcDataPtr = reinterpret_cast<const GLvoid*>(newElementArrayData.get());
    GLint size = summaryIndicesCnt * sizeof(GLuint);
    elementArrayBuffer.SetSubData(prevSize, size, srcDataPtr);
                
    GLint blockSize = sizeof(GLfloat) * 4 * 2;
    const GLvoid* offsetPtrs[] = {
        reinterpret_cast<const GLvoid*>(0),
        reinterpret_cast<const GLvoid*>(sizeof(GLfloat) * 4)
    };
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, blockSize, offsetPtrs[0]);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, blockSize, offsetPtrs[1]);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    unused.clear();
}

Program& Renderer::GetProgram() {
    return program;
}

}
}
}
