#pragma once

#include "VertexData.h"

#include <windows.h>
#include <gl/gl.h>

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Graphics {

template<typename T>
class Mesh {
public:
    Mesh(const Mesh& other);
    Mesh(const std::vector<VertexData<T>>& vertices);
    Mesh(const std::size_t size, const VertexData<T>* vertices);
    ~Mesh();

    std::size_t GetSize() const;
    const GLvoid* GetRawData();

private:
    std::vector<VertexData<T>> vertices;
    T** rawData;
};

template<typename T>
Mesh<T>::Mesh(const Mesh<T>& other) 
        : vertices(other.vertices)
        , rawData(nullptr) {}

template<typename T>
Mesh<T>::Mesh(const std::vector<VertexData<T>>& vertices) 
        : vertices(vertices)
        , rawData(nullptr) {}

template<typename T>
Mesh<T>::Mesh(const std::size_t size, const VertexData<T>* vertices) 
        : rawData(nullptr) {
    for (int i = 0; i < size; i++) {
        this->vertices.push_back(vertices[i]);
    }
}

template<typename T>
Mesh<T>::~Mesh() {
    if (rawData) {
        for (int i = 0; i < vertices.size(); i++) {
            delete[] rawData[i];
        }
        delete[] rawData;
    }
}

template<typename T>
std::size_t Mesh<T>::GetSize() const {
    return vertices.size() * vertices[0].GetSize();
}

template<typename T>
const GLvoid* Mesh<T>::GetRawData() {
    if (!rawData) {
        rawData = new T*[vertices.size()];
        int row = 0;
        for (auto& vertex : vertices) {
            int column = 0;
            rawData[row] = new T[vertex.GetCount()];
            for (auto& attribute : vertex.attributes) {
                std::copy(attribute.GetRawData(), 
                          attribute.GetRawData() + attribute.GetCount(),
                          &rawData[0][0] + row * vertex.GetCount() + column); 
                column += attribute.GetCount();
            }
            row++;
        }
    }

    return reinterpret_cast<const GLvoid*>(rawData);
}

}
}
}
