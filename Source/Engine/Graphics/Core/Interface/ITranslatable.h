#pragma once

#include "Vector.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

class ITranslatable {
public:
    virtual ~ITranslatable();

    void Translate(const Vector<T>& translatable) = 0;
};

}
}
