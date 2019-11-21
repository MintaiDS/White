#pragma once

#include "Object.h"
#include "ITransformable.h"
#include "MotionController.h"
#include "ObjectManager.h"

namespace White {
namespace Engine {

template<typename T>
class MovableObject : public Object, public ITransformable<T> {
public:
    MovableObject();

    virtual void Scale(const Vector<T>& scaling);
    virtual void Rotate(const Vector<T>& rotation);
    virtual void Translate(const Vector<T>& translation);
    virtual void Transform(const Vector<T>& scaling, 
                           const Vector<T>& rotation, 
                           const Vector<T>& translation); 

protected:
    ObjectManager& om;
    unsigned motionController; 
};

template<typename T>
MovableObject<T>::MovableObject() : om(ObjectManager::GetInstance()) {
    motionController = om.Create<MotionController<T>>();
}
    
template<typename T>
void MovableObject<T>::Scale(const Vector<T>& scaling) {
    InterfaceProvider ip;
    std::shared_ptr<IScalable> mc;
    mc = op.Query<IScalable>(om.GetObjectById(motionController));
    mc.Scale(scaling);
}

template<typename T>
void MovableObject<T>::Rotate(const Vector<T>& rotation) {
    InterfaceProvider ip;
    std::shared_ptr<IRotatable> mc;
    mc = op.Query<IRotatable>(om.GetObjectById(motionController));
    mc.Rotate(rotation);
}

template<typename T>
void MovableObject<T>::Translate(const Vector<T>& translation) {
    InterfaceProvider ip;
    std::shared_ptr<ITranslatable> mc;
    mc = op.Query<IScalable>(om.GetObjectById(motionController));
    mc.Translate(translation);
}

template<typename T>
void MovableObject<T>::Transform(const Vector<T>& scaling, 
                                 const Vector<T>& rotation, 
                                 const Vector<T>& translation) {
    InterfaceProvider ip;
    std::shared_ptr<ITransformable> mc;
    mc = op.Query<ITransformable>(om.GetObjectById(motionController));
    mc.Transform(scaling, rotation, translation);
}

}
}
