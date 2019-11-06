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
    Matrix(std::vector<std::vector<T>> values);
    Matrix(const Matrix<T>& other);

    Vector<T> operator[](std::size_t index) const;
    Vector<T>& operator[](std::size_t index);
    Matrix& operator+=(const Matrix<T>& other);
    Matrix& operator-=(const Matrix<T>& other);
    Matrix& operator*=(const Matrix<T>& other);
    Matrix& operator+=(const T value);
    Matrix& operator-=(const T value);
    Matrix& operator*=(const T value);
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
    for (int i = 0; i < rows; i++) {
        this->values[i] = std::make_unique<Vector>(columns, values[i]);
    }
}

template<typename T>
Matrix<T>::Matrix(std::vector<std::vector<T>> values)
        : rows(values.size())
        , columns(values[0].size())
        , values(std::make_unique<std::unique_ptr<Vector<T>>[]>(rows)) {
    for (int i = 0; i < rows; i++) {
        this->values[i] = std::make_unique<Vector>(values[i]);
    }
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& other) 
        : rows(other.rows),
        , columns(other.columns)
        , values(std::make_unique<std::unique_ptr<Vector<T>>[]>(rows)) {
    for (int i = 0; i < rows; i++) {
        this->values[i] = std::make_unique<Vector>(other.values[i]);
    }
}

template<typename T>
Vector<T> Matrix<T>::operator[](std::size_t index) const {
    return Vector<T>(values[index]);
}

template<typename T>
Vector<T>& Matrix<T>::operator[](std::size_t index) {
    return values[index];
}

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& other) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            values[i][j] += other[i][j];
        }
    }

    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& other) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            values[i][j] -= other[i][j];
        }
    }
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& other) {
    
}

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const T value) {
    for (auto& row : values) {
        for (auto& column : row) {
            column += value;  
        }
    }

    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator-=(const T value) {
    return *this += -value;
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const T value) {
    for (auto& row : values) {
        for (auto& column : row) {
            column *= value;
        }
    }
}

}
}
