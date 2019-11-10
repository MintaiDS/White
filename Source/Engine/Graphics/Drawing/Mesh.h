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
    std::size_t GetCount() const;
    T* GetRawData();

private:
    std::vector<VertexData<T>> vertices;
    T* rawData;
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
    delete[] rawData;
}

template<typename T>
std::size_t Mesh<T>::GetSize() const {
    return vertices.size() * vertices[0].GetSize();
}

template<typename T>
std::size_t Mesh<T>::GetCount() const {
    return vertices.size() * vertices[0].GetCount();
}

template<typename T>
T* Mesh<T>::GetRawData() {
    if (!rawData) {
        rawData = new T[vertices.size() * vertices[0].GetCount()];
        int row = 0;
        for (auto& vertex : vertices) {
            int column = 0;
            for (auto& attribute : vertex.attributes) {
                std::copy(attribute.GetRawData(), 
                          attribute.GetRawData() + attribute.GetCount(),
                          rawData + row * vertex.GetCount() + column); 
                column += attribute.GetCount();
            }
            row++;
        }
    }

    return rawData;
}

}
}
}
