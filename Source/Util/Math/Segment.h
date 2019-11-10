#pragma once

#include "Vector.h"

namespace White {
namespace Util {
namespace Math {

template<typename T>
struct Segment {
    Segment(const Vector<T>& begin, const Vector<T>& end);

    Vector<T> begin;
    Vector<T> end;
};

template<typename T>
Segment<T>::Segment(const Vector<T>& begin, const Vector<T>& end) 
        : begin(begin)
        , end(end) {}

}
}
}
