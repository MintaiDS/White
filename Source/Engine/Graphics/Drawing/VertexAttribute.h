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
    VertexAttribute(const Vector<T>& data);
    VertexAttribute(const VertexAttribute<T>& other);
    ~VertexAttribute();

    VertexAttribute& operator=(const VertexAttribute<T>& other);

    std::size_t GetSize() const;
    std::size_t GetCount() const;
    const T* GetRawData();

    Vector<T> data;
    T* rawData;
};

template<typename T>
VertexAttribute<T>::VertexAttribute() : rawData(nullptr) {}

template<typename T>
VertexAttribute<T>::VertexAttribute(const Vector<T>& data) 
        : data(data)
        , rawData(nullptr) {}

template<typename T>
VertexAttribute<T>::VertexAttribute(const VertexAttribute<T>& other) 
        : data(other.data)
        , rawData(nullptr) {}

template<typename T>
VertexAttribute<T>& VertexAttribute<T>::operator=(
                                            const VertexAttribute<T>& other) {
    data = other.data;
    rawData = nullptr;

    return *this;
}

template<typename T>
VertexAttribute<T>::~VertexAttribute() {
    delete[] rawData;
}

template<typename T>
std::size_t VertexAttribute<T>::GetSize() const {
    return sizeof(T) * data.GetSize();
}

template<typename T>
std::size_t VertexAttribute<T>::GetCount() const {
    return data.GetSize();
}

template<typename T>
const T* VertexAttribute<T>::GetRawData() {
    if (!rawData) {
        rawData = new T[data.GetSize()];
        for (int i = 0; i < data.GetSize(); i++) {
            rawData[i] = data[i];
        }
    }

    return reinterpret_cast<const T*>(rawData);
}

}
}
}
