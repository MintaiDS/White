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

InterfaceProvider::InterfaceProvider() {}

template<typename T>
std::shared_ptr<T> InterfaceProvider::Query(std::weak_ptr<Object> ptr) {
    auto sharedPtr = ptr.lock();

    return std::static_pointer_cast<T>(sharedPtr);
}

}
