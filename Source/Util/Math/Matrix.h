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

    static Matrix Identity(std::size_t dimension);
    static Matrix Diagonal(const Vector<T>& diagonal);
    static Matrix Rotation(const Vector<T>& rotation);
    static Matrix Translation(const Vector<T>& translation);
    static Matrix Scaling(const Vector<T>& scale);
    static Matrix Projection(T l, T t, T r, T b, T n, T f);
    static Matrix Projection(T fov, T aspect, T n, T f);
    static Matrix OrtographicProjection(T l, T t, T r, T b, T n, T f);
    static Matrix OrtographicProjection(T fox, T aspect, T n, T f);
    
    Matrix& Transpose();
    Matrix Transposed();
    Matrix& RemoveRow(const std::size_t row);
    Matrix& RemoveColumn(const std::size_t column);
    Matrix Inversed() const;
    Matrix& Inverse();
    T Determinant() const;
    T Minor() const;
    T Minor(const std::size_t row, const std::size_t column) const;
    Matrix& operator=(const Matrix<T>& other);
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
    Vector<T> operator*(const Vector<T>& vec) const;
    Matrix operator+(const T value) const;
    Matrix operator-(const T value) const;
    Matrix operator*(const T value) const;
    Vector<T> Row(const std::size_t row) const;
    Vector<T> Column(const std::size_t column) const;
    Vector<T>& Row(const std::size_t row);

    void UpdateDataPtr();
    T* GetDataPtr();

    bool isOutOfDate;
    std::size_t rows;
    std::size_t columns;
    std::unique_ptr<Vector<T>[]> values;
    std::unique_ptr<T[]> data;
};

template<typename T>
Matrix<T>::Matrix() : rows(0), columns(0), isOutOfDate(true) {} 

template<typename T>
Matrix<T>::Matrix(std::size_t dimension) 
        : rows(dimension)
        , columns(dimension)
        , values(std::make_unique<Vector<T>[]>(rows)) 
        , isOutOfDate(true) {
    for (std::size_t i = 0; i < rows; i++) {
        values[i] = Vector<T>(dimension);
    }
}

template<typename T>
Matrix<T>::Matrix(std::size_t rows, std::size_t columns) 
        : rows(rows)
        , columns(columns)
        , values(std::make_unique<std::unique_ptr<Vector<T>>[]>(rows)) 
        , isOutOfDate(true) {
    for (auto& row : values) {
        row = std::make_unique<Vector>(columns);
    }
}

template<typename T>
Matrix<T>::Matrix(std::size_t rows, std::size_t columns, T** values) 
        : rows(rows)
        , columns(columns)
        , values(std::make_unique<std::unique_ptr<Vector<T>>[]>(rows)) 
        , isOutOfDate(true) {
    for (std::size_t i = 0; i < rows; i++) {
        this->values[i] = std::make_unique<Vector>(columns, values[i]);
    }
}

template<typename T>
Matrix<T>::Matrix(std::vector<std::vector<T>> values)
        : rows(values.size())
        , columns(values[0].size())
        , values(std::make_unique<std::unique_ptr<Vector<T>>[]>(rows)) 
        , isOutOfDate(true) {
    for (std::size_t i = 0; i < rows; i++) {
        this->values[i] = std::make_unique<Vector>(values[i]);
    }
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& other) 
        : rows(other.rows)
        , columns(other.columns)
        , values(std::make_unique<Vector<T>[]>(rows)) 
        , isOutOfDate(true) {
    for (std::size_t i = 0; i < rows; i++) {
        this->values[i] = other.values[i];
    }
}

template<typename T>
Matrix<T>::Matrix(std::initializer_list<Vector<T>> args) 
        : rows(args.size())
        , columns(args.begin()->GetSize())
        , values(std::make_unique<Vector<T>[]>(rows))  
        , isOutOfDate(true) {
    std::initializer_list<Vector<T>>::iterator it; 
    std::size_t i = 0;
    for (it = args.begin(); it != args.end(); ++it) {
        values[i] = *it;
        ++i;
    }
}

template<typename T>
Matrix<T> Matrix<T>::Identity(std::size_t dimension) {
    Matrix<T> ret(dimension);
    for (std::size_t i = 0; i < dimension; i++) {
        ret[i][i] = 1;
    }

    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::Diagonal(const Vector<T>& diagonal)  {
    Matrix<T> ret(diagonal.GetSize());
    for (std::size_t i = 0; i < diagonal.GetSize(); i++) {
        ret[i][i] = diagonal[i];
    }

    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::Rotation(const Vector<T>& rotation) {
    Matrix<T> ret = Identity(rotation.GetSize());  
    Matrix<T> rotations[3] = {ret, ret, ret};
    rotations[0][1][1] = std::cos(ToRadians(rotation[0]));
    rotations[0][1][2] = -std::sin(ToRadians(rotation[0]));
    rotations[0][2][1] = std::sin(ToRadians(rotation[0]));
    rotations[0][2][2] = std::cos(ToRadians(rotation[0]));
    rotations[1][0][0] = std::cos(ToRadians(rotation[1]));
    rotations[1][2][0] = -std::sin(ToRadians(rotation[1]));
    rotations[1][0][2] = std::sin(ToRadians(rotation[1]));
    rotations[1][2][2] = std::cos(ToRadians(rotation[1]));
    rotations[2][0][0] = std::cos(ToRadians(rotation[2]));
    rotations[2][0][1] = -std::sin(ToRadians(rotation[2]));
    rotations[2][1][0] = std::sin(ToRadians(rotation[2]));
    rotations[2][1][1] = std::cos(ToRadians(rotation[2]));
    ret = rotations[2] * rotations[1] * rotations[0] * ret;
    
    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::Translation(const Vector<T>& translation) {
    Matrix<T> ret = Identity(translation.GetSize() + 1);
    for (std::size_t i = 0; i < translation.GetSize(); i++) {
        ret[i][translation.GetSize()] = translation[i];
    } 

    return ret;

}

template<typename T>
Matrix<T> Matrix<T>::Scaling(const Vector<T>& scale) {
    Matrix<T> ret = Diagonal(scale);

    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::Projection(T l, T t, T r, T b, T n, T f) {
    Matrix<T> ret = {{2 * n / (r - l), 0, (l + r) / (r - l), 0}, 
                     {0, 2 * n / (t - b), (b + t) / (t - b), 0}, 
                     {0, 0, -(f + n) / (f - n), -(2 * f * n) / (f - n)},
                     {0, 0, -1, 0}};

    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::Projection(T fov, T aspect, T n, T f) {
    Matrix<T> ret = {{1 / (tan(fov / 2) * aspect), 0, 0, 0}, 
                     {0, 1 / tan(fov / 2), 0, 0},
                     {0, 0, (f + n) / (f - n), (2 * f * n) / (n - f)},
                     {0, 0, 1, 0}}

    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::OrtographicProjection(T l, T t, T r, T b, T n, T f) {
    Matrix<T> ret = {{2 / (r - l), 0, 0, (l + r) / (l - r)}, 
                     {0, 2 / (t - b), 0, (b + t) / (b - t)}, 
                     {0, 0,  2 / (f - n), (f + n) / (n - f)},
                     {0, 0, 1, 0}};

    return ret;
}

template<typename T>
Matrix<T> Matrix<T>::OrtographicProjection(T fov, T aspect, T n, T f) {
    Matrix<T> ret = {{1 / (tan(fov / 2) * n * aspect), 0, 0, 0}, 
                     {0, 1 / (tan(fov / 2) * n), 0, 0},
                     {0, 0, 2 / (f - n), (n + f) / (n - f)},
                     {0, 0, 1, 0}}

    return ret;
}    

template<typename T>
Matrix<T>& Matrix<T>::Transpose() {
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = i; j < columns; j++) {
            std::swap(values[i][j], values[j][i]);
        }
    }

    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::Transposed() {
    Matrix<T> ret(*this);
    ret.Transpose();

    return ret;
}

template<typename T>
Matrix<T>& Matrix<T>::RemoveRow(const std::size_t row) {
    std::unique_ptr<Vector<T>[]> temp = std::make_unique<Vector<T>[]>(rows);
    for (std::size_t i = 0; i < rows; i++) {
        temp[i] = values[i];
    }
    values = std::make_unique<Vector<T>[]>(rows - 1); 
    for (std::size_t i = 0; i < row; i++) {
        values[i] = temp[i];
    }
    for (std::size_t i = row; i < rows - 1; i++) {
        values[i] = temp[i + 1];
    }
    rows--;

    return *this;    
}

template<typename T>
Matrix<T>& Matrix<T>::RemoveColumn(const std::size_t column) {
    std::unique_ptr<Vector<T>[]> temp = std::make_unique<Vector<T>[]>(rows);
    for (std::size_t i = 0; i < rows; i++) {
        temp[i] = values[i];
    }
    values = std::make_unique<Vector<T>[]>(rows);
    for (std::size_t i = 0; i < rows; i++) {
        values[i] = Vector<T>(columns - 1);
        for (std::size_t j = 0; j < column; j++) { 
            values[i][j] = temp[i][j];
        }
        for (std::size_t j = column; j < columns - 1; j++) {
            values[i][j] = temp[i][j + 1];
        }
    } 
    columns--;

    return *this;    
}

template<typename T>
Matrix<T> Matrix<T>::Inversed() const {
    Matrix<T> ret(*this);
    ret.Inverse();

    return ret;
}

template<typename T>
Matrix<T>& Matrix<T>::Inverse() {
    Matrix<T> temp(*this);
    T det = Determinant(); 
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < columns; j++) {
            T coeff = std::pow(-1, i + j);
            values[i][j] = coeff * temp.Minor(i, j) / det;
        }
    }
    Transpose();

    return *this;
}

template<typename T>
T Matrix<T>::Determinant() const {
    if (rows == 1) {
        return values[0][0];
    }

    T ret = 0;
    T multiplier = 1;
    Matrix<T> cur(*this);
    for (std::size_t i = 0; i < columns; i++) {
        Matrix<T> minor(cur);
        minor.RemoveRow(0);
        minor.RemoveColumn(i);
        ret += cur[0][i] * multiplier * minor.Determinant();
        multiplier = -multiplier;
    }

    return ret;
}

template<typename T>
T Matrix<T>::Minor(const std::size_t row, const std::size_t column) const {
    Matrix<T> minor(*this);
    minor.RemoveRow(row);
    minor.RemoveColumn(column);

    return minor.Determinant();  
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
    rows = other.rows;
    columns = other.columns;
    values = std::make_unique<Vector<T>[]>(rows);
    isOutOfDate = true;
    for (std::size_t i = 0; i < rows; i++) {
        this->values[i] = other.values[i];
    }

    return *this;
}

template<typename T>
Vector<T> Matrix<T>::operator[](std::size_t index) const {
    return Vector<T>(values[index]);
}

template<typename T>
Vector<T>& Matrix<T>::operator[](std::size_t index) {
    isOutOfDate = true;

    return values[index];
}

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& other) {
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < columns; j++) {
            (*this)[i][j] += other[i][j];
        }
    }

    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& other) {
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < columns; j++) {
            (*this)[i][j] -= other[i][j];
        }
    }
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& other) {
    Matrix<T> tmp(*this);
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < columns; j++) {
            (*this)[i][j] = 0;
            for (std::size_t k = 0; k < columns; k++) {
                (*this)[i][j] += tmp[i][k] * other[k][j];
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
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < columns; j++) {
            (*this)[i][j] *= value;
        }
    }

    return *this;
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
Vector<T> Matrix<T>::operator*(const Vector<T>& vec) const {
    Vector<T> ret(vec);
    for (int i = 0; i < rows; i++) {
        Vector<T> row = (*this)[i];
        ret[i] = row.Dot(vec);
    }

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
    return *this[row];
}

template<typename T>
Vector<T> Matrix<T>::Column(const std::size_t column) const {
    std::vector<T> column;
    for (std::size_t i = 0; i < row; i++) {
        column.push_back((*this)[i][column]);
    }
    Vector<T> ret(column);

    return ret;
}

template<typename T>
Vector<T>& Matrix<T>::Row(const std::size_t row) {
    return *this[row];
}

template<typename T>
void Matrix<T>::UpdateDataPtr() {
    data.reset(new T[rows * columns]);
    for (int i = 0; i < rows; i++) {
        //std::copy(values[i].GetDataPtr(), 
        //          values[i].GetDataPtr() + columns, 
        //          data.get() + (columns * i));
        for (int j = 0; j < columns; j++) {
            data.get()[columns * i + j] = values[i][j];
        }
    }
    isOutOfDate = false;
}

template<typename T>
T* Matrix<T>::GetDataPtr() {
    if (isOutOfDate) {
        UpdateDataPtr();
    }

    return data.get();
}

}
}
}
