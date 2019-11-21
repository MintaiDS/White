#pragma once

#include "Object.h"
#include "ITransformable.h"
#include "MotionController.h"

namespace White {
namespace Engine {

template<typename T>
class MovableObject : public Object, public ITransformable<T> {
public:
    virtual void Scale(const Vector<T>& scaling);
    virtual void Rotate(const Vector<T>& rotation);
    virtual void Translate(const Vector<T>& translation);
    virtual void Transform(const Vector<T>& scaling, 
                           const Vector<T>& rotation, 
                           const Vector<T>& translation); 

protected:
    MotionController<T> motionController; 
};

template<typename T>
void MovableObject<T>::Scale(const Vector<T>& scaling) {
    motionController.Scale(scaling);
}

template<typename T>
void MovableObject<T>::Rotate(const Vector<T>& rotation) {
    motionController.Rotate(rotation);
}

template<typename T>
void MovableObject<T>::Translate(const Vector<T>& translation) {
    motionController.Translate(translation);
}

template<typename T>
void MovableObject<T>::Transform(const Vector<T>& scaling, 
                                    const Vector<T>& rotation, 
                                    const Vector<T>& translation) {
    motionController.Transform(scaling, rotation, translation);
}

}
}
