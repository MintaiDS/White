#pragma once

#include "VertexAttribute.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Graphics {

template<typename T>
struct VertexData {
    VertexData(const std::vector<VertexAttribute<T>>& attributes);

    std::size_t GetSize() const;
    std::size_t GetCount() const;

    std::vector<VertexAttribute<T>> attributes;
};

template<typename T>
VertexData<T>::VertexData(const std::vector<VertexAttribute<T>>& attributes) 
        : attributes(attributes) {}

template<typename T>
std::size_t VertexData<T>::GetSize() const {
    std::size_t ret = 0;
    for (const auto& attribute : attributes) {
        ret += attribute.GetSize();
    }

    return ret;
}

template<typename T>
std::size_t VertexData<T>::GetCount() const {
    std::size_t ret = 0;
    for (const auto& attribute : attributes) {
        ret += attribute.GetCount();
    }

    return ret;
}

}
}
}
