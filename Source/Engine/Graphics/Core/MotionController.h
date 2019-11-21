#pragma once

#include "ITransformable.h"

namespace White {
namespace Engine {

template<typename T>
class MotionController : public Controller, public ITransformable {
public:
    MotionController();

    virtual void Scale(const Vector<float>& scaling);
    virtual void Rotate(const Vector<float>& rotation);
    virtual void Translate(const Vector<float>& translation);
    virtual void Transform(const Vector<float>& scaling, 
                           const Vector<float>& rotation, 
                           const Vector<float>& translation); 

protected:
    Vector<float> scaling;
    Vector<float> rotation;
    Vector<float> translation;
};

template<typename T>
MotionController<T>::MotionController() 
        : scaling{1, 1, 1},
        , rotation{0, 0, 0}
        , translation{0, 0, 0} {}

template<typename T>
void MotionController<T>::Scale(const Vector<float>& scaling) {
    this->scaling = scaling;
}

template<typename T>
void MotionController<T>::Rotate(const Vector<float>& rotation) {
    rotation += rotation;
}

template<typename T>
void MotionController<T>::Translate(const Vector<float>& translation) {
    translation += translation;
}

template<typename T>
void MotionController<T>::Transform(const Vector<float>& scaling, 
                                    const Vector<float>& rotation, 
                                    const Vector<float>& translation) {
    Scale(scaling);
    Rotate(rotation);
    Translate(translation);
}

}
}
