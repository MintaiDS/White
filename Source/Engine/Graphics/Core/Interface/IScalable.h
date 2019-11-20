#pragma once

#include "Vector.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

class IScalable {
public:
    virtual ~IScalable();

    void Scale(const Vector<T>& scaling) = 0;
};

}
}
