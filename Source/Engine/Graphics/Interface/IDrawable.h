#pragma once

#include "Vector.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Graphics {

template<typename T>
struct IDrawable {
    void Rotate(const Vector<T>& rotation);
    void Scale(const Vector<T>& scaling);
    void Translate(const Vector<T>& translation);
};

template<typename T>
void IDrawable<T>::Rotate(const Vector<T>& rotation) {}

template<typename T>
void IDrawable<T>::Scale(const Vector<T>& rotation) {}

template<typename T>
void IDrawable<T>::Translate(const Vector<T>& rotation) {}

}
}
}
