#pragma once

#include "Vector.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

class IScalable {
public:
    virtual ~IScalable();

    template<typename T>
    void Scale(const Vector<T>& scaling); 

private:
    virtual void Scale(const Vector<float>& scaling) = 0;
};

//IScalable::~IScalable() {}

template<typename T>
void IScalable::Scale(const Vector<T>& scaling) {
    Scale(scaling);
}

}
}
