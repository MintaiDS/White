#pragma once

#include "Vector.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

class IRotatable {
public:
    void Rotate(const Vector<T>& rotation) = 0;
};

}
}
