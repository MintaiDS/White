#pragma once

#include "ObjectIDProvider.h"
#include "ITransformable.h"
#include "VertexData.h"
#include "IDrawable.h"
#include "Polygon.h"
#include "Matrix.h"
#include "Object.h"

#include <windows.h>
#include <gl/gl.h>

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Graphics {

template<typename T, typename U = unsigned>
class Mesh : public Object, ITransformable<T> {
public:
    Mesh();
    Mesh(const Mesh<T, U>& other);
    Mesh(const std::vector<VertexData<T>>& vertices);
    Mesh(const std::size_t size, const VertexData<T>* vertices);
    ~Mesh();

    std::size_t GetSize() const;
    std::size_t GetCount() const;
    unsigned GetID() const;

    Matrix<T> GetRotationMatrix() const;
    Matrix<T> GetScalingMatrix() const;
    Matrix<T> GetTranslationMatrix() const;
    Vector<T> GetRotation() const;
    Vector<T> GetScaling() const;
    Vector<T> GetTranslation() const;

    virtual void Scale(const Vector<T>& scaling);
    virtual void Rotate(const Vector<T>& rotation);
    virtual void Translate(const Vector<T>& translation);
    virtual void Transform(const Vector<T>& scaling, 
                           const Vector<T>& rotation, 
                           const Vector<T>& translation); 

    T* GetRawData();
    U* GetRawIndices();

    std::vector<VertexData<T>> vertices;
    std::vector<U> indices;

protected:
    unsigned id; 
    T* rawData;
    U* rawIndices;
    Vector<T> rotation;
    Vector<T> scaling;
    Vector<T> translation;
};

template<typename T, typename U = unsigned>
Mesh<T, U>::Mesh() 
        : rawData(nullptr)
        , rawIndices(nullptr)
        , rotation({0, 0, 0})
        , translation({0, 0, 0})
        , scaling({1, 1, 1}) {}

template<typename T, typename U = unsigned>
Mesh<T, U>::Mesh(const Mesh<T, U>& other) 
        : vertices(other.vertices)
        , indices(other.indices)
        , rawData(nullptr)
        , rawIndices(nullptr) 
        , rotation(other.rotation)
        , translation(other.translation)
        , scaling(other.scaling) {}

template<typename T, typename U = unsigned>
Mesh<T, U>::Mesh(const std::vector<VertexData<T>>& vertices) 
        : vertices(vertices)
        , rawData(nullptr)
        , rawIndices(nullptr) 
        , rotation({0, 0, 0})
        , translation({0, 0, 0})
        , scaling({1, 1, 1}) {}

template<typename T, typename U = unsigned>
Mesh<T, U>::Mesh(const std::size_t size, const VertexData<T>* vertices) 
        : id(ObjectIDProvider::GetNextID())
        , rawData(nullptr) 
        , rotation({0, 0, 0})
        , translation({0, 0, 0})
        , scaling({1, 1, 1}) {
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
std::size_t Mesh<T, U>::GetID() const {
    return id;
}

template<typename T, typename U = unsigned>
Matrix<T> Mesh<T, U>::GetRotationMatrix() const {
    return Matrix::Rotation(rotation);
}

template<typename T, typename U = unsigned>
Matrix<T> Mesh<T, U>::GetScalingMatrix() const {
    return Matrix::Scaling(scaling);
}

template<typename T, typename U = unsigned>
Matrix<T> Mesh<T, U>::GetTranslationMatrix() const {
    return Matrix::Translation(translation);
}

template<typename T, typename U = unsigned>
Vector<T> Mesh<T, U>::GetRotation() const {
    Vector<T> ret(rotation);

    return ret;
}

template<typename T, typename U = unsigned>
Vector<T> Mesh<T, U>::GetScaling() const {
    Vector<T> ret(scaling);

    return ret;
}

template<typename T, typename U = unsigned>
Vector<T> Mesh<T, U>::GetTranslation() const {
    Vector<T> ret(translation);

    return ret;
}

template<typename T, typename U = unsigned>
void Mesh<T, U>::Scale(const Vector<T>& scaling) {
    this->scaling = scaling; 
}

template<typename T, typename U = unsigned>

void Mesh<T, U>::Rotate(const Vector<T>& rotation) {
    this->rotation += rotation;
}

template<typename T, typename U = unsigned>
void Mesh<T, U>::Translate(const Vector<T>& translation) {
    this->translation += translation;
}

template<typename T, typename U = unsigned>
void Mesh<T, U>::Transform(const Vector<T>& scaling, 
                           const Vector<T>& rotation, 
                           const Vector<T>& translation) {
    Scale(scaling);
    Rotate(rotation);
    Translate(translation);
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
