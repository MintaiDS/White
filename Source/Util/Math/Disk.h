#pragma once

#include "Circle.h"

namespace White {
namespace Util {
namespace Math {

template<typename T>
struct Disk : public Circle<T> {
    Disk(T r);
};

template<typename T>
Disk<T>::Disk(T r) : Circle(r) {}

}
}
}

