#pragma once

#include "Vector.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

class ITransformable : public IScalable, 
                       public IRotatable, public ITranslatable {
public:
    virtual ~ITransformable();

    void Transform(const Vector<T>& scaling, 
                   const Vector<T>& rotation, 
                   const Vector<T>& translation) = 0;
};

}
}
