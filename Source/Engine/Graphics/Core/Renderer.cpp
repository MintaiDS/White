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
        indicesCnt += list[i].indices.size();
    }
    DrawCall(indicesCnt);
}

void Renderer::DrawCall(int indicesCnt) {
    glDrawElements(GL_TRIANGLES, indicesCnt, 
                GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(nullptr)); 
}

void Renderer::UpdateData(Mesh<GLfloat> mesh) { 
    GLint prevSize = arrayBuffer.GetSize();
    GLint newSize = prevSize + mesh.GetSize();
    GLint prevCnt = prevSize / sizeof(GLfloat); 
    GLint newCnt = prevCnt + (newSize - prevSize) / sizeof(GLfloat);
    GLfloat* newArrayData = mesh.GetRawData();
    GLfloat* oldArrayData = nullptr;
    oldArrayData = new GLfloat[prevCnt];
    arrayBuffer.GetSubData(0, prevSize, 
                           reinterpret_cast<GLvoid*>(oldArrayData));
    arrayBuffer.SetData(newSize, 
                    reinterpret_cast<const GLvoid*>(nullptr), GL_DYNAMIC_DRAW);
    arrayBuffer.SetSubData(0, prevSize, 
                           reinterpret_cast<const GLvoid*>(oldArrayData));
    arrayBuffer.SetSubData(prevSize, (newSize - prevSize), 
                           reinterpret_cast<const GLvoid*>(newArrayData));
    GLint prevArrayCnt = prevCnt / 8;
    
    prevSize = elementArrayBuffer.GetSize();
    newSize = prevSize + sizeof(GLuint) * mesh.indices.size();
    prevCnt = prevSize / sizeof(GLuint);
    newCnt = prevCnt + (newSize - prevSize) / sizeof(GLuint);
    GLuint* newElementArrayData = mesh.GetRawIndices();

    std::ofstream file("out.txt", std::ios::app);
    for (int i = 0; i < mesh.indices.size(); i++) {
        newElementArrayData[i] += prevArrayCnt;
        file << newElementArrayData[i] << " ";
    }
    file << std::endl;
    file << "prev: " << prevArrayCnt << std::endl;
    file.close();
    GLuint* oldElementArrayData = new GLuint[prevCnt];
    elementArrayBuffer.GetSubData(0, prevSize, 
                    reinterpret_cast<GLvoid*>(oldElementArrayData));
    elementArrayBuffer.SetData(newSize, 
                reinterpret_cast<const GLvoid*>(nullptr), GL_DYNAMIC_DRAW);
    elementArrayBuffer.SetSubData(0, prevSize,
                    reinterpret_cast<const GLvoid*>(oldElementArrayData));
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
     
    delete[] oldArrayData;
    delete[] oldElementArrayData;     
}

void Renderer::AddMesh(Mesh<GLfloat> mesh) {
    list.push_back(mesh);
    UpdateData(mesh);
}

}
}
}
