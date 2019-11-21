#pragma once

#include "ITransformable.h"

namespace White {
namespace Engine {

template<typename T>
class MotionController : public Controller, public ITransformable<T> {
public:
    MotionController();

    virtual void Scale(const Vector<T>& scaling);
    virtual void Rotate(const Vector<T>& rotation);
    virtual void Translate(const Vector<T>& translation);
    virtual void Transform(const Vector<T>& scaling, 
                           const Vector<T>& rotation, 
                           const Vector<T>& translation); 

protected:
    Vector<T> scaling;
    Vector<T> rotation;
    Vector<T> translation;
};

template<typename T>
MotionController<T>::MotionController() 
        : scaling{1, 1, 1},
        , rotation{0, 0, 0}
        , translation{0, 0, 0} {}

template<typename T>
void MotionController<T>::Scale(const Vector<T>& scaling) {
    this->scaling = scaling;
}

template<typename T>
void MotionController<T>::Rotate(const Vector<T>& rotation) {
    rotation += rotation;
}

template<typename T>
void MotionController<T>::Translate(const Vector<T>& translation) {
    translation += translation;
}

template<typename T>
void MotionController<T>::Transform(const Vector<T>& scaling, 
                                    const Vector<T>& rotation, 
                                    const Vector<T>& translation) {
    Scale(scaling);
    Rotate(rotation);
    Translate(translation);
}

}
}
