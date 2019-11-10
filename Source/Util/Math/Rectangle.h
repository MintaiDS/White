#pragma once

namespace White {
namespace Util {
namespace Math {

template<typename T>
struct Rectangle {
    Rectangle(const T w, const T h);

    T w;
    T h;
};

template<typename T>
Rectangle<T>::Rectangle(const T w, const T h) : w(w), h(h) {}

}
}
}

