#include "Renderer.h"

#include <fstream>
#include <iostream>

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

void Renderer::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    indicesCnt = 0;
    for (int i = 0; i < list.size(); i++) {
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
