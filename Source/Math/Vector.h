#include <vector>
#include <memory>

namespace White {
namespace Math {

template<typename T>
struct Vector {          
    Vector();
    Vector(size_t size, T* values);
    Vector(std::vector<T> values);

    std::size_t size;
    std::unique_ptr<T*> values;
};

template<typename T>
Vector<T>::Vector() : size(0) {}

template<typename T>
Vector<T>::Vector(std::size_t size, T* values) 
        : size(size)
        , values(std::make_unique<T[]>(size)) {
    std::copy(values, values + size, this->values);
}

template<typename T>
Vector<T>::Vector(std::vector<T> values) 
        : size(values.size())
        , values(std::make_unique<T[]>(size)) { 
    std::copy(std::begin(values), std::end(values), this->values);
}

}
}
