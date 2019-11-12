#include "GLFunctions.h"
#include "Renderer.h"
#include "Disk.h"
#include "Ring.h"

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
    Shader shader(GL_VERTEX_SHADER);
    std::wstring path = L"Engine/Shaders/default.vsh";
    shader.Source(path);
    shader.Compile();
    program.Attach(shader);
    //shader.Delete();
    path = L"Engine/Shaders/default.fsh";
    shader.Create(GL_FRAGMENT_SHADER);
    shader.Source(path);
    shader.Compile();
    program.Attach(shader);
    //shader.Delete();
    program.Link();
    program.Use();
    //program.Delete();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW); 
    Vector<GLfloat> color = {1.0f, 1.0f, 0.0f, 1.0f};
    Vector<GLfloat> colorBorder = {1.0f, 0.0f, 1.0f, 1.0f};
    for (int i = 0; i < 10; i++) {
        Disk<GLfloat> disk(0.2);
        Mesh<GLfloat> mesh = disk.ToMesh(color, 720);
        mesh.Scale({0.2f, 0.2f, 1.0f});
        mesh.Translate({0.18f * i, 0.18f * i, 0.0f});
        AddMesh(mesh);
        Ring<GLfloat> ring(0.1, 0.28);
        mesh = ring.ToMesh(colorBorder, 720);
        mesh.Scale({0.2f, 0.2f, 1.0f});
        mesh.Translate({0.18f * i, 0.18f * i, 0.0f});
        AddMesh(mesh);
    }
}

void Renderer::Render() {
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
        //translation += {0.5f, 0.0f, 0.0f, 0.0f};
        Matrix<GLfloat> translationMatrix
            = Matrix<GLfloat>::Translation(translation);
        Vector<GLfloat> scaling = mesh.GetScaling();
        //scaling *= 0.1f;
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
        GLint location = glGetUniformLocation(program.id, "model");
        glUniformMatrix4fv(location, 1, GL_TRUE, raw.get());
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
    list.push_back(mesh);
    UpdateData(mesh);
}

}
}
}
