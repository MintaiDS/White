#pragma once

#include "Vector.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

template<typename T>
class ITranslatable {
public:
    virtual ~ITranslatable();

    virtual void Translate(const Vector<T>& translatable) = 0;
};

}
}
