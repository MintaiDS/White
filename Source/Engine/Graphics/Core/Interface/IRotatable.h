#pragma once

#include "Vector.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

template<typename T>
class IRotatable {
public:
    virtual ~IRotatable();

    void Rotate(const Vector<T>& rotation) = 0;
};

}
}
