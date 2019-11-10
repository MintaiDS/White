#pragma once

namespace White {
namespace Util {
namespace Math {

template<typename T>
struct Ellipse {
    Ellipse(const T w, const T h);

    T w;
    T h;
};

template<typename T>
Ellipse<T>::Ellipse(const T w, const T h) : w(w), h(h) {}

}
}
}

