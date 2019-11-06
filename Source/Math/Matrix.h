#pragma once

#include <memory>
#include <algorithm>

#include "Vector.h"

namespace White {
namespace Math {

template<typename T>
struct Matrix {
    Matrix();
    Matrix(std::size_t rows, std::size_t columns);
    Matrix(std::size_t rows, std::size_t columns, T** values);
    //Matrix(std::vector<std::vector<T>> values);
    //Matrix(const Matrix<T>& other);

    //T[] operator[](std::size_t index) const;
    //T[]& operator[](std::size_t index);
    //Matrix& operator+=(const Matrix<T>& other);
    //Matrix& operator-=(const Matrix<T>& other);
    //Matrix& operator+=(const T value);
    //Matrix& operator-=(const T value);
    //Matrix& operator*=(const T value);
    //Matrix operator+(const Matrix<T>& other) const;
    //Matrix operator-(const Matrix<T>& other) const;
    //Matrix operator+(const T value) const;
    //Matrix operator-(const T value) const;
    //Matrix operator*(const T value) const;

    //T dot(const Matrix<T>& other) const;
    //Matrix cross(const Matrix<T>& other) const;
    //T mixed(const Matrix<T>& otherFirst, const Matrix<T>& otherSecond) const;

    //std::size_t size;
    //std::unique_ptr<T[][]> values; 
    std::size_t rows;
    std::size_t columns;
    std::unique_ptr<Vector<T>[]> values;
};

template<typename T>
Matrix<T>::Matrix() : rows(0), columns(0) {} 

template<typename T>
Matrix<T>::Matrix(std::size_t rows, std::size_t columns) 
        : rows(rows)
        , columns(columns)
        , values(std::make_unique<std::unique_ptr<Vector<T>>[]>(rows)) {
    for (auto& row : values) {
        row = std::make_unique<Vector>(columns);
    }
}

template<typename T>
Matrix<T>::Matrix(std::size_t rows, std::size_t columns, T** values) 
        : rows(rows)
        , columns(columns)
        , values(std::make_unique<std::unique_ptr<Vector<T>>[]>(rows)) {
    for (auto& row : values) {
        row = std::make_unique<Vector>(columns, row);
    }
}

}
}
