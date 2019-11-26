#pragma once

namespace White {
namespace Util {
namespace Math {

constexpr double pi = 3.14159265358979;

template<typename T>
T ToRadians(T angle) {
    return angle * pi / 180.0;
}

template<typename T>
T ToDegrees(T angle) {
    return angle * 180.0 / pi;
}

}
}
}
