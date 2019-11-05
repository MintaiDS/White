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

    T& operator[](std::size_t index);
    T& operator+=(const Vector<T>& other);
    T& operator-=(const Vector<T>& other);
    T& operator*=(const Vector<T>& other);
    T& operator+=(const T value);
    T& operator-=(const T value);
    T& operator*=(const T value);
    T operator+(const Vector<T>& other);
    T operator-(const Vector<T>& other);
    T operator*(const Vector<T>& other);
    T operator+(const T value);
    T operator-(const T value);
    T operator*(const T value);

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
T& Vector<T>::operator[](std::size_t index) {
    return values[index];
}

}
}
