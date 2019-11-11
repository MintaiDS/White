#pragma once

#include <memory>
#include <algorithm>

#include "Vector.h"

namespace White {
namespace Util {
namespace Math {

template<typename T>
struct Matrix {
    Matrix();
    Matrix(std::size_t dimension);
    Matrix(std::size_t rows, std::size_t columns);
    Matrix(std::size_t rows, std::size_t columns, T** values);
    Matrix(std::vector<std::vector<T>> values);
    Matrix(const Matrix<T>& other);
    Matrix(std::initializer_list<Vector<T>> args);

    static Matrix<T> Identity(std::size_t dimension);
    static Matrix<T> Diagonal(const Vector<T>& diagonal);
    static Matrix<T> Rotation(const Vector<T>& rotation);
    static Matrix<T> Translation(const Vector<T>& translation);
    static Matrix<T> Scale(const Vector<T>& scale);
    Vector<T> operator[](std::size_t index) const;
    Vector<T>& operator[](std::size_t index);
    Matrix& operator+=(const Matrix<T>& other);
    Matrix& operator-=(const Matrix<T>& other);
    Matrix& operator*=(const Matrix<T>& other);
    Matrix& operator+=(const T value);
    Matrix& operator-=(const T value);
    Matrix& operator*=(const T value);
    Matrix operator+(const Matrix<T>& other) const;
    Matrix operator-(const Matrix<T>& other) const;
    Matrix operator*(const Matrix<T>& other) const;
    Matrix operator+(const T value) const;
    Matrix operator-(const T value) const;
    Matrix operator*(const T value) const;
    Vector<T> Row(const std::size_t row) const;
    Vector<T> Column(const std::size_t column) const;
    Vector<T>& Row(const std::size_t row);

    T Determinant() const;
    T Minor() const;


    std::size_t rows;
    std::size_t columns;
    std::unique_ptr<Vector<T>[]> values;
};

template<typename T>
Matrix<T>::Matrix() : rows(0), columns(0) {} 

template<typename T>
Matrix<T>::Matrix(std::size_t dimension) 
        : rows(dimension)
        , columns(dimension)
        , values(std::make_unique<std::unique_ptr<Vector<T>>[]>(rows)) { 
    for (auto& row : values) {
        row = std::make_unique<Vector>(columns);
    }
}

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
Matrix<T>::Matrix(std::initializer_list<Vector<T>> args) 
        : rows(args.size())
        , columns(*args.begin().size)
        , values(std::make_unique<std::unique_ptr<Vector<T>>[]>(rows)) {
    std::initializer_list<T>::iterator it; 
    int i = 0;
    for (it = args.begin(); it != args.end(); ++it) {
        values[i] = *it;
        ++i;
    }
}

template<typename T>
Matrix<T> Matrix<T>::Identity(std::size_t dimension) {
    Matrix<T> ret(dimension);
    for (int i = 0; i < dimension; i++) {
        ret[i][i] = 1;
    }

    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::Diagonal(const Vector<T>& diagonal)  {
    Matrix<T> ret(diagonal.size);
    for (int i = 0; i < diagonal.size; i++) {
        values[i][i] = diagonal[i];
    }

    return ret
}

template<typename T>
Matrix<T> Matrix<T>::Rotation(const Vector<T>& rotation) {
    Matrix<T> ret = Identity(rotation.size);  
    Matrix<T> rotations[3] = {ret, ret, ret};
    rotations[0][1][1] = std::cos(rotation[0]);
    rotations[0][1][2] = -std::sin(rotation[0]);
    rotations[0][2][1] = std::sin(rotation[0]);
    rotations[0][2][2] = std::cos(rotation[0]);
    rotations[1][0][0] = std::cos(rotation[0]);
    rotations[1][2][0] = -std::sin(rotation[0]);
    rotations[1][0][2] = std::sin(rotation[0]);
    rotations[1][2][2] = std::cos(rotation[0]);
    rotations[2][0][0] = std::cos(rotation[0]);
    rotations[2][0][1] = -std::sin(rotation[0]);
    rotations[2][1][0] = std::sin(rotation[0]);
    rotations[2][1][1] = std::cos(rotation[0]);
    ret = rotations[2] * rotations[1] * rotations[0] * ret;
    
    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::Translation(const Vector<T>& translation) {
    Matrix<T> ret = Identity(translation.size);

    return ret;

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
    Matrix<T> tmp(*this);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            *this[i][j] = 0;
            for (int k = 0; k < columns; k++) {
                *this[i][j] += *this[i][k] * other[k][j];
            }
        }
    }

    return *this;
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

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const {
    Matrix<T> ret(*this);
    ret += other;

    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const {
    Matrix<T> ret(*this);
    ret -= other;

    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const {
    Matrix<T> ret(*this);
    ret *= other;

    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const T value) const {
    Matrix<T> ret(*this);
    ret += value;

    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const T value) const {
    return *this + -value;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const T value) const {
    Matrix<T> ret(*this);
    ret *= value;

    return ret;
}

template<typename T>
Vector<T> Matrix<T>::Row(const std::size_t row) const {
    return values[row];
}

template<typename T>
Vector<T> Matrix<T>::Column(const std::size_t column) const {
    std::vector<T> column;
    for (int i = 0; i < row; i++) {
        column.push_back(values[i][column]);
    }
    Vector<T> ret(column);

    return ret;
}

template<typename T>
Vector<T>& Matrix<T>::Row(const std::size_t row) {
    return values[row];
}

}
}
}
