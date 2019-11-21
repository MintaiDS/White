#pragma once

#include "Object.h"
#include "ITransformable.h"
#include "MotionController.h"
#include "ObjectManager.h"

namespace White {
namespace Engine {

template<typename T>
class MovableObject : public Object, public ITransformable {
public:
    MovableObject();

    virtual void Scale(const Vector<float>& scaling);
    virtual void Rotate(const Vector<float>& rotation);
    virtual void Translate(const Vector<float>& translation);
    virtual void Transform(const Vector<float>& scaling, 
                           const Vector<float>& rotation, 
                           const Vector<float>& translation); 

protected:
    ObjectManager& om;
    unsigned motionController; 
};

template<typename T>
MovableObject<T>::MovableObject() : om(ObjectManager::GetInstance()) {
    motionController = om.Create<MotionController<T>>();
}
    
template<typename T>
void MovableObject<T>::Scale(const Vector<float>& scaling) {
    InterfaceProvider ip;
    std::shared_ptr<IScalable> mc;
    mc = op.Query<IScalable>(om.GetObjectById(motionController));
    mc.Scale(scaling);
}

template<typename T>
void MovableObject<T>::Rotate(const Vector<float>& rotation) {
    InterfaceProvider ip;
    std::shared_ptr<IRotatable> mc;
    mc = op.Query<IRotatable>(om.GetObjectById(motionController));
    mc.Rotate(rotation);
}

template<typename T>
void MovableObject<T>::Translate(const Vector<float>& translation) {
    InterfaceProvider ip;
    std::shared_ptr<ITranslatable> mc;
    mc = op.Query<IScalable>(om.GetObjectById(motionController));
    mc.Translate(translation);
}

template<typename T>
void MovableObject<T>::Transform(const Vector<float>& scaling, 
                                 const Vector<float>& rotation, 
                                 const Vector<float>& translation) {
    InterfaceProvider ip;
    std::shared_ptr<ITransformable> mc;
    mc = op.Query<ITransformable>(om.GetObjectById(motionController));
    mc.Transform(scaling, rotation, translation);
}

}
}
