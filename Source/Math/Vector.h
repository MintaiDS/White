#pragma once

#include <vector>
#include <memory>
#include <algorithm>

namespace White {
namespace Math {

template<typename T>
struct Vector {          
    Vector();
    Vector(std::size_t size);
    Vector(std::size_t size, T* values);
    Vector(std::vector<T> values);
    Vector(const Vector<T>& other);

    T operator[](std::size_t index) const;
    T& operator[](std::size_t index);
    Vector& operator+=(const Vector<T>& other);
    Vector& operator-=(const Vector<T>& other);
    Vector& operator+=(const T value);
    Vector& operator-=(const T value);
    Vector& operator*=(const T value);
    Vector operator+(const Vector<T>& other) const;
    Vector operator-(const Vector<T>& other) const;
    Vector operator+(const T value) const;
    Vector operator-(const T value) const;
    Vector operator*(const T value) const;

    T dot(const Vector<T>& other) const;
    Vector cross(const Vector<T>& other) const;
    T mixed(const Vector<T>& otherFirst, const Vector<T>& otherSecond) const;

    std::size_t size;
    std::unique_ptr<T[]> values;
};

template<typename T>
Vector<T>::Vector() : size(0) {}

template<typename T>
Vector<T>::Vector(std::size_t size) 
        : size(size)
        , values(std::make_unique<T[]>(size)) {
    for (auto& value : values) {
        value = 0;
    }
}

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
        values[i] = other[i];
    }
}

template<typename T>
T Vector<T>::operator[](std::size_t index) const {
    return values[index];
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
Vector<T> Vector<T>::operator+(const Vector<T>& other) const {
    Vector<T> ret(*this);
    ret += other;

    return ret;
}

template<typename T>
Vector<T> Vector<T>::operator-(const Vector<T>& other) const {
    Vector<T> ret(*this);
    ret -= other;

    return ret;
}

template<typename T>
Vector<T> Vector<T>::operator+(const T value) const {
    Vector<T> ret(*this);
    ret += value;

    return ret;
}

template<typename T>
Vector<T> Vector<T>::operator-(const T value) const {
    return *this + -value;
}

template<typename T>
Vector<T> Vector<T>::operator*(const T value) const {
    Vector<T> ret(*this);
    ret *= value;

    return ret;
}

template<typename T>
T Vector<T>::dot(const Vector<T>& other) const {
    T ret = 0;
    for (int i = 0; i < size; i++) {
        ret += *this[i] * other[i];
    }

    return ret;
}

template<typename T>
Vector<T> Vector<T>::cross(const Vector<T>& other) const {
    Vector<T> ret(size);
    ret[0] = *this[1] * other[2] - *this[2] * other[1];
    ret[1] = *this[0] * other[2] - *this[2] * other[0];
    ret[2] = *this[0] * other[1] - *this[1] * other[0];

    return ret;
}

template<typename T>
T Vector<T>::mixed(const Vector<T>& otherFirst, 
                   const Vector<T>& otherSecond) const {
    return *this.dot(otherFrist.cross(otherSecond));
}

}
}
