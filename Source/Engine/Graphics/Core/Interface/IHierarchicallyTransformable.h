#pragma once

#include "ITransformable.h"

namespace White {
namespace Engine {

class IHierarchicallyTransformable : public ITransformable {
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
