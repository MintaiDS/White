#pragma once

#include "ITransformable.h"

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
};

}
}
