#pragma once

#include "Mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace White {
namespace Engine {
namespace Graphics {

void MeshLoader::Import(std::wstring path) {
    std::ifstream file(path);
    int verticesCnt = 0;
    int attributesCnt = 0;
    std::vector<int> componentsCnts;
    file >> verticesCnt;
    file >> attributesCnt;
    for (int i = 0; i < attributesCnt; i++) {
        int componentsCnt = 0;
        file >> componentsCnt;
        componentsCnts.push_back(componentsCnt);
    }
    std::vector<VertexData<GLfloat>> vertices;
    for (int i = 0; i < verticesCnt; i++) {
        std::vector<VertexAttribute<GLfloat>> attributes;
        for (int j = 0; j < attributesCnt; j++) {
            std::vector<GLfloat> components[componentsCnts[j]];
            for (int k = 0; k < componentsCnts[j]; k++) {
                file >> components[k];
            }
            Vector<GLfloat> componentsVector(components);
            VertexAttribute<GLfloat> vertexAttribute(components);
            attributes.push_back(vertexAttribute);
        }
        VertexData<GLfloat> vertexData(attributes);
        vertices.push_back(vertexData);
    }
    file.close();
}

void MeshLoader::Export(std::wstring path) {
}

};

}
}
}
