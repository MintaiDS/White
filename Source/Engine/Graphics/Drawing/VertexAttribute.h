#pragma once

#include "Vector.h"

#include <algorithm>

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Graphics {

template<typename T>
struct VertexAttribute {
    VertexAttribute();
    ~VertexAttribute();

    std::size_t GetSize() const;
    std::size_t GetCount() const;
    const void* GetRawData();

    Vector<T> data;
    T* rawData;
};

template<typename T>
VertexAttribute<T>::VertexAttribute() : rawData(nullptr) {}

template<typename T>
VertexAttribute<T>::~VertexAttribute() {
    delete[] rawData;
}

template<typename T>
std::size_t VertexAttribute<T>::GetSize() const {
    return sizeof(T) * data.Size();
}

template<typename T>
std::size_t VertexAttribute<T>::GetCount() const {
    return data.Size();
}

template<typename T>
const void* VertexAttribute<T>::GetRawData() {
    if (!rawData) {
        rawData = new T[data.Size()];
        for (int i = 0; i < data.Size(); i++) {
            rawData[i] = data[i];
        }
    }

    return reinterpret_cast<const void*>(rawData);
}

}
}
}
