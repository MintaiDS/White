#pragma once

#include "ObjectIDProvider.h"
#include "InterfaceProvider.h"
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
class Mesh : public Object, public ITransformable {
public:
    Mesh();
    Mesh(const Mesh<T, U>& other);
    Mesh(const std::vector<VertexData<T>>& vertices);
    Mesh(const std::size_t size, const VertexData<T>* vertices);
    ~Mesh();

    std::size_t GetSize() const;
    std::size_t GetCount() const;
    unsigned GetID() const;
    bool IsTransformed() const;
    void ToggleTransform();

    Matrix<T> GetRotationMatrix() const;
    Matrix<T> GetScalingMatrix() const;
    Matrix<T> GetTranslationMatrix() const;
    Vector<T> GetRotation() const;
    Vector<T> GetScaling() const;
    Vector<T> GetTranslation() const;

    virtual void Scale(const Vector<float>& scaling);
    virtual void Rotate(const Vector<float>& rotation);
    virtual void Translate(const Vector<float>& translation);
    virtual void Transform(const Vector<float>& scaling, 
                           const Vector<float>& rotation, 
                           const Vector<float>& translation); 

    Matrix<float>& GetTransformationMatrix();

    T* GetRawData();
    U* GetRawIndices();

    std::vector<VertexData<T>> vertices;
    std::vector<U> indices;

protected:
    void UpdateTransformationMatrix();
    void UpdateTransformationMatrix(Matrix<float> parental);

    struct {
        Matrix<float> parental;
        Matrix<float> that;
    } transformation;

    Matrix<float> parentalTransformationMatrix;
    Matrix<float> transformationMatrix;
    unsigned id; 
    T* rawData;
    U* rawIndices;
    Vector<float> rotation;
    Vector<float> scaling;
    Vector<float> translation;
    bool isTransformed;
};

template<typename T, typename U = unsigned>
Mesh<T, U>::Mesh() 
        : rawData(nullptr)
        , rawIndices(nullptr)
        , rotation({0, 0, 0})
        , translation({0, 0, 0})
        , scaling({1, 1, 1}) {
    UpdateTransformationMatrix();
    transformation.parental = Matrix<float>::Identity(4);
    UpdateTransformationMatrix(transformation.parental);
}

template<typename T, typename U = unsigned>
Mesh<T, U>::Mesh(const Mesh<T, U>& other) 
        : vertices(other.vertices)
        , indices(other.indices)
        , rawData(nullptr)
        , rawIndices(nullptr) 
        , rotation(other.rotation)
        , translation(other.translation)
        , scaling(other.scaling) {
    UpdateTransformationMatrix();
    transformation.parental = other.transformation.parental;
    UpdateTransformationMatrix(transformation.parental);
}

template<typename T, typename U = unsigned>
Mesh<T, U>::Mesh(const std::vector<VertexData<T>>& vertices) 
        : vertices(vertices)
        , rawData(nullptr)
        , rawIndices(nullptr) 
        , rotation({0, 0, 0})
        , translation({0, 0, 0})
        , scaling({1, 1, 1}) {
    UpdateTransformationMatrix();
    transformation.parental = Matrix<float>::Identity(4);
    UpdateTransformationMatrix(transformation.parental);
}

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
    UpdateTransformationMatrix();
    transformation.parental = Matrix<float>::Identity(4);
    UpdateTransformationMatrix(transformation.parental);
}

template<typename T, typename U = unsigned>
Mesh<T, U>::~Mesh() {
    delete[] rawData;
    delete[] rawIndices;
}

template<typename T, typename U = unsigned> 
bool Mesh<T, U>::IsTransformed() const {
    return isTransformed;
}

template<typename T, typename U = unsigned> 
void Mesh<T, U>::ToggleTransform() {
    isTransformed = !isTransformed;    
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
    UpdateTransformationMatrix();
    return Matrix::Rotation(rotation);
}

template<typename T, typename U = unsigned>
Matrix<T> Mesh<T, U>::GetScalingMatrix() const {
    //UpdateTransformationMatrix();
    return Matrix::Scaling(scaling);
}

template<typename T, typename U = unsigned>
Matrix<T> Mesh<T, U>::GetTranslationMatrix() const {
    //UpdateTransformationMatrix();
    return Matrix::Translation(translation);
}

template<typename T, typename U = unsigned>
Vector<T> Mesh<T, U>::GetRotation() const {
    //UpdateTransformationMatrix();
    Vector<T> ret(rotation);

    return ret;
}

template<typename T, typename U = unsigned>
Vector<T> Mesh<T, U>::GetScaling() const {
    //UpdateTransformationMatrix();
    Vector<T> ret(scaling);

    return ret;
}

template<typename T, typename U = unsigned>
Vector<T> Mesh<T, U>::GetTranslation() const {
    //UpdateTransformationMatrix();
    Vector<T> ret(translation);

    return ret;
}

template<typename T, typename U = unsigned>
void Mesh<T, U>::Scale(const Vector<float>& scaling) {
    this->scaling = scaling; 
    isTransformed = true;
    //UpdateTransformationMatrix();
    UpdateTransformationMatrix(transformation.parental);
}

template<typename T, typename U = unsigned>
void Mesh<T, U>::Rotate(const Vector<float>& rotation) {
    this->rotation += rotation;
    isTransformed = true;
    //UpdateTransformationMatrix();
    UpdateTransformationMatrix(transformation.parental);
}

template<typename T, typename U = unsigned>
void Mesh<T, U>::Translate(const Vector<float>& translation) {
    this->translation += translation;
    isTransformed = true;
    //UpdateTransformationMatrix();
    UpdateTransformationMatrix(transformation.parental);
}

template<typename T, typename U = unsigned>
void Mesh<T, U>::Transform(const Vector<float>& scaling, 
                           const Vector<float>& rotation, 
                           const Vector<float>& translation) {
    Scale(scaling);
    Rotate(rotation);
    Translate(translation);
    isTransformed = true;
    //UpdateTransformationMatrix();
    UpdateTransformationMatrix(transformation.parental);
}

template<typename T, typename U = unsigned>
Matrix<float>& Mesh<T, U>::GetTransformationMatrix() {
    //UpdateTransformationMatrix();
    return transformation.that;//transformationMatrix;
}

template<typename T, typename U = unsigned>
void Mesh<T, U>::UpdateTransformationMatrix() {
    Matrix<float> scalingMatrix = Matrix<float>::Scaling(scaling);
    Matrix<float> rotationMatrix = Matrix<float>::Rotation(rotation); 
    Matrix<float> translationMatrix = Matrix<float>::Translation(translation);
    scalingMatrix 
        = {{scalingMatrix[0][0], scalingMatrix[0][1], scalingMatrix[0][2], 0},
           {scalingMatrix[1][0], scalingMatrix[1][1], scalingMatrix[1][2], 0}, 
           {scalingMatrix[2][0], scalingMatrix[2][1], scalingMatrix[2][2], 0},
           {0, 0, 0, 1.0f}};
   rotationMatrix 
        = {{rotationMatrix[0][0], 
            rotationMatrix[0][1], rotationMatrix[0][2], 0},
           {rotationMatrix[1][0], 
            rotationMatrix[1][1], rotationMatrix[1][2], 0}, 
           {rotationMatrix[2][0], 
            rotationMatrix[2][1], rotationMatrix[2][2], 0},
           {0, 0, 0, 1.0f}};
    transformationMatrix = translationMatrix * rotationMatrix * scalingMatrix;
}

template<typename T, typename U = unsigned>
void Mesh<T, U>::UpdateTransformationMatrix(Matrix<float> parental) {
    //Matrix<float> scalingMatrix = Matrix<float>::Scaling(scaling);
    //Matrix<float> rotationMatrix = Matrix<float>::Rotation(rotation); 
    //Matrix<float> translationMatrix = Matrix<float>::Translation(translation);
    //scalingMatrix 
    //    = {{scalingMatrix[0][0], scalingMatrix[0][1], scalingMatrix[0][2], 0},
    //       {scalingMatrix[1][0], scalingMatrix[1][1], scalingMatrix[1][2], 0}, 
    //       {scalingMatrix[2][0], scalingMatrix[2][1], scalingMatrix[2][2], 0},
    //       {0, 0, 0, 1.0f}};
    //rotationMatrix 
    //    = {{rotationMatrix[0][0], 
    //        rotationMatrix[0][1], rotationMatrix[0][2], 0},
    //       {rotationMatrix[1][0], 
    //        rotationMatrix[1][1], rotationMatrix[1][2], 0}, 
    //       {rotationMatrix[2][0], 
    //        rotationMatrix[2][1], rotationMatrix[2][2], 0},
    //       {0, 0, 0, 1.0f}};
    //transformationMatrix = translationMatrix * rotationMatrix * scalingMatrix;
    UpdateTransformationMatrix();
    transformation.parental = parental;
    transformation.that = parental * transformationMatrix;
    InterfaceProvider ip;
    for (int i = 0; i < childs.size(); i++) {
        auto mesh = ip.Query<Mesh<float>>(childs[i]);
        if (mesh) {
            mesh->UpdateTransformationMatrix(transformation.that);
        }
    }
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
