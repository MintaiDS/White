#pragma once

#include "Circle.h"

namespace White {
namespace Util {
namespace Math {

typedef<typename T>
struct Ring {
    Ring(const Circle<T>& inner, const Circle<T>& outer);
    
    Circle<T>& inner;
    Circle<T>& outer;
};

template<typename T>
Ring<T>::Ring(const Circle<T>& inner, const Circle<T>& outer) 
        : inner(inner)
        , outer(outer) {}

}
}
}

