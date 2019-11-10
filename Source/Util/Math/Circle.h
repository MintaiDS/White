#pragma once

#include "Ellipse.h"

namespace White {
namespace Util {
namespace Math {

template<typename T>
struct Circle : public Ellipse<T> {
    Circle(T r);

    T r;
};

template<typename T>
Circle<T>::Circle(const T r) 
        : Ellipse(r, r)
        , r(r) {}

}
}
}

