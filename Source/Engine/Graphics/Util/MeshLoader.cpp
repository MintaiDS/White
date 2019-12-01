#pragma once

#include "MeshLoader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace White {
namespace Engine {
namespace Graphics {

MeshLoader::MeshLoader() {}

MeshLoader::MeshLoader(const Mesh<GLfloat>& mesh) : mesh(mesh) {}

Mesh<float> MeshLoader::Import(std::wstring path) {
    std::ifstream file(path);
    int verticesCnt = 0;
    int attributesCnt = format.numAttributes;
    int indicesCnt = 0;
    std::vector<int> componentsCnts;
    file >> verticesCnt;
    file >> indicesCnt;
    for (int i = 0; i < attributesCnt; i++) {
        componentsCnts.push_back(format.numComponents[i]);
    }
    std::vector<VertexData<GLfloat>> vertices;
    for (int i = 0; i < verticesCnt; i++) {
        std::vector<VertexAttribute<GLfloat>> attributes(3);
        for (int j = 0; j < attributesCnt; j++) {
            std::vector<GLfloat> components(componentsCnts[j]);
            for (int k = 0; k < componentsCnts[j]; k++) {
                file >> components[k];
            }
            Vector<GLfloat> componentsVector(components);
            VertexAttribute<GLfloat> vertexAttribute(componentsVector);
            attributes.push_back(vertexAttribute);
        }
        VertexData<GLfloat> vertexData(attributes);
        vertices.push_back(vertexData);
    }
    Mesh<float> imported(vertices);
    for (int i = 0; i < indicesCnt; i++) {
        int index;
        file >> index;
        imported.indices.push_back(index);
    }
    mesh = imported;
    file.close();

    return mesh;
}

void MeshLoader::Export(std::wstring path, const Mesh<GLfloat>& mesh) {
    std::ofstream file(path);
    int verticesCnt = mesh.vertices.size();
    int attributesCnt = mesh.vertices[0].attributes.size();
    int indicesCnt = mesh.indices.size();
    std::vector<int> componentsCnts;
    file << verticesCnt << std::endl;
    file << attributesCnt << std::endl;
    for (int i = 0; i < attributesCnt; i++) {
        int componentsCnt = mesh.vertices[0].attributes[i].GetCount();
        file << componentsCnt << " ";
    }
    file << std::endl;
    for (int i = 0; i < verticesCnt; i++) {
        for (int j = 0; j < attributesCnt; j++) {
            for (int k = 0; k < mesh.vertices[i].attributes[j].GetCount(); k++) {
                file << mesh.vertices[i].attributes[j].data[k] << " ";
            }
            file << std::endl;
        }
    }
    for (int i = 0; i < indicesCnt; i++) {
        file << mesh.indices[i] << " ";
    }
    file.close();
}

}
}
}
