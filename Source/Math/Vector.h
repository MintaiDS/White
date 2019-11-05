#pragma once

#include <vector>
#include <memory>
#include <algorithm>

namespace White {
namespace Math {

template<typename T>
struct Vector {          
    Vector();
    Vector(std::size_t size, T* values);
    Vector(std::vector<T> values);
    Vector(const Vector<T>& other);

    T& operator[](std::size_t index);
    Vector& operator+=(const Vector<T>& other);
    Vector& operator-=(const Vector<T>& other);
    Vector& operator+=(const T value);
    Vector& operator-=(const T value);
    Vector& operator*=(const T value);
    Vector operator+(const Vector<T>& other);
    Vector operator-(const Vector<T>& other);
    Vector operator+(const T value);
    Vector operator-(const T value);
    Vector operator*(const T value);
    Vector dot(const Vector<T>& other);

    std::size_t size;
    std::unique_ptr<T[]> values;
};

template<typename T>
Vector<T>::Vector() : size(0) {}

template<typename T>
Vector<T>::Vector(std::size_t size, T* values) 
        : size(size)
        , values(std::make_unique<T[]>(size)) {
    for (int i = 0; i < size; i++) {
        this->values[i] = values[i];
    }
}

template<typename T>
Vector<T>::Vector(std::vector<T> values) 
        : size(values.size())
        , values(std::make_unique<T[]>(size)) { 
    for (int i = 0; i < size; i++) {
        this->values[i] = values[i];
    }
}

template<typename T>
Vector<T>::Vector(const Vector<T>& other)
        : size(other.size)
        , values(std::make_unique<T[]>(size)) {
    for (int i = 0; i < size; i++) {
        this->values[i] = values[i];
    }
}

template<typename T>
T& Vector<T>::operator[](std::size_t index) {
    return values[index];
}

template<typename T>
Vector<T>& Vector<T>::operator+=(const Vector<T>& other) {
    for (int i = 0; i < size; ++i) {
        values[i] += other[i];
    }

    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator-=(const Vector<T>& other) {
    for (int i = 0; i < size; ++i) {
        values[i] -= other[i];
    }

    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator+=(const T value) {
    for (int i = 0; i < size; ++i) {
        values[i] += value;
    }

    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator-=(const T value) {
    return *this += -value;
}

template<typename T>
Vector<T>& Vector<T>::operator*=(const T value) {
    for (int i = 0; i < size; i++) {
        values[i] *= value;
    }

    return *this;
}

template<typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& other) {
    Vector<T> ret(*this);
    ret += other;

    return ret;
}

template<typename T>
Vector<T> Vector<T>::operator-(const Vector<T>& other) {
    Vector<T> ret(*this);
    ret -= other;

    return ret;
}

template<typename T>
Vector<T> Vector<T>::operator+(const T value) {
    Vector<T> ret(*this);
    ret += value;

    return ret;
}

template<typename T>
Vector<T> Vector<T>::operator-(const T value) {
    return *this + -value;
}

template<typename T>
Vector<T> Vector<T>::operator*(const T value) {
    Vector<T> ret(*this);
    ret *= value;

    return ret;
}

template<typename T>
Vector<T> Vector<T>::dot(const Vector<T>& other) {
    T ret = 0;
    for (int i = 0; i < size; i++) {
        ret += *this[i] * other[i];
    }

    return ret;
}

}
}
