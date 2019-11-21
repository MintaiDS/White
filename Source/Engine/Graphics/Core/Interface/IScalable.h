#pragma once

#include "Vector.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

template<typename T>
class IScalable {
public:
    virtual ~IScalable();

    virtual void Scale(const Vector<T>& scaling) = 0;
};

template<typename T>
IScalable<T>::~IScalable() {}

}
}
