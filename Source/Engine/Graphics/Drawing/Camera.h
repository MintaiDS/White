#pragma once

#include "ITransformable.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

template<typename T>
class Camera : public ITransformable {
public:
    Camera();

    virtual void Scale(const Vector<<T>& scaling);
    virtual void Rotate(const Vector<T>& rotation);
    virtual void Translate(const Vector<T>& translation);
    virtual void Transform(const Vector<T>& scaling, 
                           const Vector<T>& rotation, 
                           const Vector<T>& translation);

protected:
    Vector<T> position;   
};

}
}
