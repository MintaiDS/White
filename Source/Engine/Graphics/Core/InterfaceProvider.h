#pragma once

#include "Object.h"

#include <memory>

namespace White {

class InterfaceProvider {
public:
    InterfaceProvider();

    template<typename T> 
    std::shared_ptr<T> Query(std::weak_ptr<Object> ptr);
};

template<typename T>
std::shared_ptr<T> InterfaceProvider::Query(std::weak_ptr<Object> ptr) {
    auto sharedPtr = ptr.lock();

    return std::dynamic_pointer_cast<T>(sharedPtr);
}

}
