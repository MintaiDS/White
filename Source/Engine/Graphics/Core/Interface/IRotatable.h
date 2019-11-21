#pragma once

#include "Vector.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

class IRotatable {
public:
    virtual ~IRotatable();

    template<typename T>
    void Rotate(const Vector<T>& rotation);

private:
    virtual void Rotate(const Vector<float>& rotation) = 0; 
};

template<typename T>
void IRotatable::Rotate(const Vector<T>& rotation) {
    Rotate(rotation);
}

}
}
