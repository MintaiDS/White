#pragma once

#include "Vector.h"

namespace White {
namespace Engine {
namespace Math {

template<typename T>
struct IDrawable {
    void Rotate(const Vector<T>& rotation);
    void Scale(const Vector<T>& scaling);
    void Translate(const Vector<T>& translation);
};

template<typename T>
void IDrawable::Rotate(const Vector<T>& rotation) {}

template<typename T>
void IDrawable::Scale(const Vector<T>& rotation) {}

template<typename T>
void IDrawable::Translate(const Vector<T>& rotation) {}

}
}
}
