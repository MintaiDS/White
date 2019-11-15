#include "GLFunctions.h"
#include "Renderer.h"
#include "Disk.h"
#include "Ring.h"
#include "Rectangle.h"
#include "Game.h"
#include "GraphVisualizer.h"
#include "Logger.h"

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
    White::Util::Logger& logger = White::Util::Logger::GetInstance();
    //logger.Init(L"shader-log.txt");
    //logger.Flush();

    program.Create();
    Shader shader(GL_VERTEX_SHADER);
    std::wstring path = L"Engine/Shaders/default.vsh";
    shader.Source(path);
    shader.Compile();

    //std::ofstream out("shader-log.txt", std::ios::app);
    //out << shader.GetSource() << std::endl;
    //out << shader.GetSourceLength() << std::endl;
    //out << shader.IsCompiled() << std::endl;
    //out << shader.GetInfoLog() << std::endl;
    //out.close();

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
    //Vector<GLfloat> color = {1.0f, 1.0f, 0.0f, 1.0f};
    //Vector<GLfloat> colorBorder = {1.0f, 0.0f, 1.0f, 1.0f};
    //Util::Math::Rectangle<GLfloat> rect(5, 0.4);
    //Mesh<GLfloat> rectMesh 
    //    = rect.ToMesh(color + Vector<GLfloat>{-0.2, -0.2, 0.3, 0.0f}, 0);
    //rectMesh.Scale({0.3f, 0.3f, 1.0f});
    //rectMesh.Translate({0.6f, 0.6f, 0.3f});
    //rectMesh.Rotate({0.0f, 0.0f, 20.0f});
    //Disk<GLfloat> disk(0.2);
    //Mesh<GLfloat> diskMesh = disk.ToMesh(color, 720);
    //diskMesh.Scale({0.2f, 0.2f, 1.0f});
    //diskMesh.Translate({-0.97f, -0.97f, 0.0f});
    //Ring<GLfloat> ring(0.1, 0.28);
    //Mesh<GLfloat> ringMesh = ring.ToMesh(colorBorder, 720);
    //ringMesh.Scale({0.2f, 0.2f, 1.0f});
    //ringMesh.Translate({-0.97f, -0.97f, 0.0f});
    //AddMesh(rectMesh);
    //for (int i = 0; i < 10; i++) {
    //    ringMesh.Translate({0.18f, 0.18f, 0.0f});
    //    diskMesh.Translate({0.18f, 0.18f, 0.0f});
    //    AddMesh(diskMesh);
    //    AddMesh(ringMesh);
    //}
    //game = std::make_shared<GraphVizualizer>(*this);
    //games.push_back(game);
    game = new GraphVisualizer(*this);
}

void Renderer::Render() {
    //for (int i = 0; i < games.size(); i++) {
    //    games[i]->Play();
    //}
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

        //std::ofstream out("log.txt", std::ios::app);
        std::unique_ptr<GLfloat[]> raw 
            = std::make_unique<GLfloat[]>(model.rows * model.columns);
        for (int i = 0; i < model.rows; i++) {
            for (int j = 0; j < model.columns; j++) {
                raw.get()[i * model.columns + j] = model[i][j];
            }
        }
        program.Use();
        GLint location = glGetUniformLocation(program.id, "model");
        //out << "Model uniform location: " << location << std::endl;

        program.Use();
        glProgramUniformMatrix4fv(program.id, location, 1, GL_TRUE, raw.get());
        Matrix<GLfloat> view = Matrix<GLfloat>::Identity(4);
        program.Use();
        location = glGetUniformLocation(program.id, "view");
        //out << "View uniform location: " << location << std::endl;
        for (int i = 0; i < view.rows; i++) {
            for (int j = 0; j < view.columns; j++) {
                raw.get()[i * view.columns + j] = view[i][j];
                //out << raw.get()[i * view.columns + j] <<  " ";
            }
            //out << std::endl;
        }

        //out.close();

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
    list.push_back(mesh);
    UpdateData(mesh);
}

}
}
}
