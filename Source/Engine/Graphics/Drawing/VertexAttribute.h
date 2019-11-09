#pragma once

#include "Vector.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Graphics {

template<typename T>
struct VertexAttribute {
    std::size_t GetSize() const;
    std::size_t GetCount() const;

    Vector<T> data;
};

template<typename T>
std::size_t VertexAttribute<T>::GetSize() const {
    return sizeof(T) * data.Size();
}

template<typename T>
std::size_t VertexAttribute<T>::GetCount() const {
    return data.Size();
}

}
}
}
