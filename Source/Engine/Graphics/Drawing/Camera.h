#pragma once

#include "ITransformable.h"
#include "Matrix.h"

#include <sstream>
#include <fstream>

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
    //Vector<T> d = {0, 0, -1};
    Vector<T> d = {0, 0, 1};
    //d = Matrix<T>::Rotation({rotation[0], rotation[1], rotation[2]}) * d;
    //d = {d[0], d[1], d[2]};
    Vector<T> u = {0, 1, 0};
    Vector<T> r = d.Cross(u);
    u = d.Cross(r); 
    Matrix<T> view = {{r[0], u[0], d[0]},
                      {r[1], u[1], d[1]},
                      {r[2], u[2], d[2]}}; 
    view = Matrix<T>::Rotation({rotation[0], rotation[1], rotation[2]}) * view;
    //view.Transpose();
    //view.Transpose();
    //view.Transpose(); 
    //view = Matrix<T>::Rotation(rotation) * view;
    view.Inverse();
    view = {{view[0][0], view[0][1], view[0][2], 0},
            {view[1][0], view[1][1], view[1][2], 0},
            {view[2][0], view[2][1], view[2][2], 0},
            {0, 0, 0, 1}};
    Matrix<T> t = Matrix<T>::Translation(translation * -1); 
    //view.Transpose();
    //view.Inverse();
    //view.Transpose();
    //view.Inverse();
    //std::ofstream out ("log.txt", std::ios::app);
    //out << "View matrix:\n";
    //view = view * t;
    //for (int i = 0; i < view.rows; i++) {
    //    for (int j = 0; j < view.columns; j++) {
    //        out << view[i][j] << " ";
    //    }
    //    out << std::endl;
    //}
    //out.close();

    //view = Matrix<T>::Identity(4);

    return view * t;
}

}
}
