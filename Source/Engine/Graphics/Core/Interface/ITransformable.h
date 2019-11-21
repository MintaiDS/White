#pragma once

#include "IScalable.h"
#include "IRotatable.h"
#include "ITranslatable.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

template<typename T>
class ITransformable : public IScalable<T>, 
                       public IRotatable<T>, public ITranslatable<T> {
public:
    virtual ~ITransformable();

    virtual void Transform(const Vector<T>& scaling, 
                           const Vector<T>& rotation, 
                           const Vector<T>& translation) = 0;
};

}
}
