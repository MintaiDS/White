#pragma once

namespace White {
namespace Util {
namespace Math {

template<typename T>
struct Color {
    Color(T r = 0, T g = 0, T b = 0, T a = 0);

    T& r = rgba[0];
    T& g = rgba[1];
    T& b = rgba[2];
    T& a = rgba[3];
    T rgba[4];
};

template<typename T>
Color<T>::Color(T r, T g, T b, T a) : r(r), g(g), b(b), a(a) {}

}
}
}
