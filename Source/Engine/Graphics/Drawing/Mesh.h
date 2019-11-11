#pragma once

#include "VertexData.h"
#include "IDrawable.h"
#include "Polygon.h"
#include "Matrix.h"

#include <windows.h>
#include <gl/gl.h>

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Graphics {

template<typename T, typename U = unsigned>
class Mesh : public IDrawable<T> {
public:
    Mesh();
    Mesh(const Mesh<T, U>& other);
    Mesh(const std::vector<VertexData<T>>& vertices);
    Mesh(const std::size_t size, const VertexData<T>* vertices);
    ~Mesh();

    std::size_t GetSize() const;
    std::size_t GetCount() const;

    Matrix<T> GetRotation() const;
    Matrix<T> GetScaling() const;
    Matrix<T> GetTranslation() const;

    void Rotate(const Vector<T>& rotation);
    void Scale(const Vector<T>& scaling);
    void Translate(const Vector<T>& translation);

    T* GetRawData();
    U* GetRawIndices();

    std::vector<VertexData<T>> vertices;
    std::vector<U> indices;

protected:
        T* rawData;
        U* rawIndices;
        Vector<T> rotation;
        Vector<T> scaling;
        Vector<T> translation;
};

template<typename T, typename U = unsigned>
Mesh<T, U>::Mesh() : rawData(nullptr), rawIndices(nullptr) {}

template<typename T, typename U = unsigned>
Mesh<T, U>::Mesh(const Mesh<T, U>& other) 
        : vertices(other.vertices)
        , indices(other.indices)
        , rawData(nullptr)
        , rawIndices(nullptr) {}

template<typename T, typename U = unsigned>
Mesh<T, U>::Mesh(const std::vector<VertexData<T>>& vertices) 
        : vertices(vertices)
        , rawData(nullptr)
        , rawIndices(nullptr) {}

template<typename T, typename U = unsigned>
Mesh<T, U>::Mesh(const std::size_t size, const VertexData<T>* vertices) 
        : rawData(nullptr) {
    for (int i = 0; i < size; i++) {
        this->vertices.push_back(vertices[i]);
    }
}

template<typename T, typename U = unsigned>
Mesh<T, U>::~Mesh() {
    delete[] rawData;
    delete[] rawIndices;
}

//template<typename T, typename U = unsigned>
//Mesh<T, U> Mesh<T, U>::CreateFromShape(
//        const Util::Math::Rectangle<T>& rectangle, const Vector<T>& color) {
//    Polygon<T> polygon = Polygon::CreateFromShape(rectangle);
//    std::vector<VertexData<T>> vertices; 
//    for (int i = 0; i < polygon.vertices.size(); i++) {
//        VertexData<T> vertex;
//        Vector<T> position = {polygon.vertices[i][0], 
//                              polygon.vertices[i][1],
//                              0, 1};
//        vertex.attributes.push_back(position);
//        vertex.attributes.push_back(color);
//        vertices.push_back(vertex);
//    }
//    Mesh<T, U> ret(vertices);
// 
//    return ret;
//}
//
//template<typename T, typename U = unsigned>
//Mesh<T, U> Mesh<T, U>::CreateFromShape(
//                        const Segment<T>& segment, const Vector<T>& color) {
//    std::vector<VertexData<T>> vertices;    
//    Mesh<T, U> ret(vertices);
//
//    return ret;
//}

template<typename T, typename U = unsigned>
std::size_t Mesh<T, U>::GetSize() const {
    return vertices.size() * vertices[0].GetSize();
}

template<typename T, typename U = unsigned>
std::size_t Mesh<T, U>::GetCount() const {
    return vertices.size() * vertices[0].GetCount();
}

template<typename T, typename U = unsigned>
Matrix<T> Mesh<T, U>::GetRotation() const {
    return Matrix::Rotation(rotation);
}

template<typename T, typename U = unsigned>
Matrix<T> Mesh<T, U>::GetScaling() const {
    return Matrix::Scaling(scaling);
}

template<typename T, typename U = unsigned>
Matrix<T> Mesh<T, U>::GetTranslation() const {
    return Matrix::Translation(translation);
}

template<typename T, typename U = unsigned>
void Mesh<T, U>::Rotate(const Vector<T>& rotation) {
    this->rotation += rotation; 
}

template<typename T, typename U = unsigned>
void Mesh<T, U>::Scale(const Vector<T>& scaling) {
    this->scaling += scaling;
}

template<typename T, typename U = unsigned>
void Mesh<T, U>::Translate(const Vector<T>& translation) {
    this->translation += translation;
}

template<typename T, typename U = unsigned>
T* Mesh<T, U>::GetRawData() {
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

template<typename T, typename U = unsigned>
U* Mesh<T, U>::GetRawIndices() {
    if (!rawIndices) {
        rawIndices = new U[indices.size()];
        for (int i = 0; i < indices.size(); i++) {
            rawIndices[i] = indices[i];
        }
    }

    return rawIndices;
}

}
}
}
