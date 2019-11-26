#pragma once

#include "Segment.h"

namespace White {
namespace Util {
namespace Math {

template<typename T>
struct Line : public Segment<T> {
    Line(const Vector<T>& begin, const Vector<T>& end);
};

template<typename T>
Line<T>::Line(const Vector<T>& begin, const Vector<T>& end) 
        : Segment(begin, end) {}

}
}
}

