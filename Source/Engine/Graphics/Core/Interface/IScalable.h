#pragma once

#include "Vector.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

class IScalable {
public:
    void Scale(const Vector<T>& scaling) = 0;
};

}
}
