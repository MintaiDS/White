#pragma once

#include "IScalable.h"
#include "IRotatable.h"
#include "ITranslatable.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

class ITransformable : public IScalable,
                       public IRotatable, 
                       public ITranslatable {
public:
    virtual ~ITransformable();

    template<typename T>
    void Transform(const Vector<T>& scaling, 
                   const Vector<T>& rotation, 
                   const Vector<T>& translation);

private:
    virtual void Transform(const Vector<float>& scaling, 
                           const Vector<float>& rotation, 
                           const Vector<float>& translation) = 0;
};

template<typename T>
void ITransformable::Transform(const Vector<T>& scaling, 
                               const Vector<T>& rotation, 
                               const Vector<T>& translation) {
    Transform(scaling, rotation, translation);
}

}
}
