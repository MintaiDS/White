#pragma once

#include "ITransformable.h"
#include "Matrix.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

template<typename T>
class Camera : public ITransformable {
public:
    Camera();

    virtual void Scale(const Vector<float>& scaling);
    virtual void Rotate(const Vector<float>& rotation);
    virtual void Translate(const Vector<float>& translation);
    virtual void Transform(const Vector<float>& scaling, 
                           const Vector<float>& rotation, 
                           const Vector<float>& translation);

    Matrix<T> GetViewMatrix() const;

protected:
    Vector<T> scaling;
    Vector<T> rotation;
    Vector<T> translation; 
};

template<typename T>
Camera<T>::Camera() 
        : scaling({1, 1, 1})
        , rotation({0, 0, 0})
        , translation({0, 0, 0}) {}

template<typename T>
void Camera<T>::Scale(const Vector<float>& scaling) {
    this->scaling = scaling; 
}

template<typename T>
void Camera<T>::Rotate(const Vector<float>& rotation) {
    this->rotation += rotation;
}

template<typename T>
void Camera<T>::Translate(const Vector<float>& translation) {
    this->translation += translation;
}

template<typename T>
void Camera<T>::Transform(const Vector<float>& scaling, 
                           const Vector<float>& rotation, 
                           const Vector<float>& translation) {
    Scale(scaling);
    Rotate(rotation);
    Translate(translation);
}

template<typename T>
Matrix<T> Camera<T>::GetViewMatrix() const {
    Vector<T> d = {0, 0, 1};
    Vector<T> u = {0, 1, 0};
    Vector<T> r = u.Cross(d);
    u = r.Cross(d);
    Matrix<T> view = Matrix<T>{r, u, d}.Transposed();
    view = Matrix<T>::Rotation(rotation) * Matrix<T>::Scaling(scaling) * view;
    view.Inverse();
    view = {{view[0][0], view[0][1], view[0][2], -translation[0]},
            {view[1][0], view[1][1], view[1][2], -translation[1]},
            {view[2][0], view[2][1], view[2][2], -translation[2]},
            {0, 0, 0, 1}};

    return view;
}

}
}
