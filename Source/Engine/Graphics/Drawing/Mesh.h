#pragma once

#include "VertexData.h"
#include "Polygon.h"

#include <windows.h>
#include <gl/gl.h>

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Graphics {

template<typename T>
class Mesh {
public:
    Mesh();
    Mesh(const Mesh<T>& other);
    Mesh(const std::vector<VertexData<T>>& vertices);
    Mesh(const std::size_t size, const VertexData<T>* vertices);
    ~Mesh();

    static Mesh<T> CreateFromShape(const Util::Math::Ellipse<T>& ellipse, 
                                   const Vector<T>& color, int verticesCnt);
    static Mesh<T> CreateFromShape(const Disk<T>& disk, 
                                   const Vector<T>& color, int verticesCnt);
    static Mesh<T> CreateFromShape(const Ring<T>& ring, 
                                   const Vector<T>& color, int verticesCnt);
    static Mesh<T> CreateFromShape(
            const Util::Math::Rectangle<T>& rectangle, const Vector<T>& color);
    static Mesh<T> CreateFromShape(
                        const Segment<T>& segment, const Vector<T>& color);

    std::size_t GetSize() const;
    std::size_t GetCount() const;
    T* GetRawData();

    std::vector<VertexData<T>> vertices;

private:
        T* rawData;
};

template<typename T>
Mesh<T>::Mesh() : rawData(nullptr) {}

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
Mesh<T> Mesh<T>::CreateFromShape(const Util::Math::Ellipse<T>& ellipse, 
                                 const Vector<T>& color, int verticesCnt) {
    Util::Math::Polygon<T> polygon = Util::Math::Polygon::CreateFromShape(ellipse, verticesCnt);
    std::vector<VertexData<T>> vertices;
    for (int i = 0; i < polygon.vertices.size(); i++) {
        VertexData<T> vertex;
        Vector<T> position = {polygon.vertices[i][0], 
                              polygon.vertices[i][1],
                              0, 1};
        vertex.attributes.push_back(position);
        vertex.attributes.push_back(color);
        vertices.push_back(vertex);
    }
    Mesh<T> ret(vertices);

    return ret;
}

template<typename T>
Mesh<T> Mesh<T>::CreateFromShape(const Disk<T>& disk, 
                                 const Vector<T>& color, int verticesCnt) {
    Polygon<T> polygon = Polygon::CreateFromShape(disk, verticesCnt);
    std::vector<VertexData<T>> vertices;
    vertices.push_back({{0, 0, 0, 1}, color});
    for (int i = 0; i < polygon.vertices.size(); i++) {
        VertexData<T> vertex;
        Vector<T> position = {polygon.vertices[i][0], 
                              polygon.vertices[i][1],
                              0, 1};
        vertex.attributes.push_back(position);
        vertex.attributes.push_back(color);
        vertices.push_back(vertex);
    }
    Mesh<T> ret(vertices);
   
    return ret;
}

template<typename T>
Mesh<T> Mesh<T>::CreateFromShape(const Ring<T>& ring, 
                                 const Vector<T>& color, int verticesCnt) {
    Polygon<T> polygon = Polygon::CreateFromShape(ring, verticesCnt);
    std::vector<VertexData<T>> vertices; 
    for (int i = 0; i < polygon.vertices.size(); i++) {
        VertexData<T> vertex;
        Vector<T> position = {polygon.vertices[i][0], 
                              polygon.vertices[i][1],
                              0, 1};
        vertex.attributes.push_back(position);
        vertex.attributes.push_back(color);
        vertices.push_back(vertex);
    }
    Mesh<T> ret(vertices);

    return ret;
}

template<typename T>
Mesh<T> Mesh<T>::CreateFromShape(
        const Util::Math::Rectangle<T>& rectangle, const Vector<T>& color) {
    Polygon<T> polygon = Polygon::CreateFromShape(rectangle);
    std::vector<VertexData<T>> vertices; 
    for (int i = 0; i < polygon.vertices.size(); i++) {
        VertexData<T> vertex;
        Vector<T> position = {polygon.vertices[i][0], 
                              polygon.vertices[i][1],
                              0, 1};
        vertex.attributes.push_back(position);
        vertex.attributes.push_back(color);
        vertices.push_back(vertex);
    }
    Mesh<T> ret(vertices);
 
    return ret;
}

template<typename T>
Mesh<T> Mesh<T>::CreateFromShape(
                        const Segment<T>& segment, const Vector<T>& color) {
    std::vector<VertexData<T>> vertices;    
    Mesh<T> ret(vertices);

    return ret;
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
