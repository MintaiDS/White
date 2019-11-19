#pragma once

#include "Object.h"

#include <memory>

namespace White {

class InterfaceProvider {
public:
    InterfaceProvider(std::weak_ptr<Object> ptr);

    template<typename T> std::shared_ptr<T> Query();

protected:
    std::weak_ptr<Object> ptr;
};

InterfaceProvider::InterfaceProvider(std::weak_ptr<Object> ptr) : ptr(ptr) {}

template<typename T>
std::shared_ptr<T> InterfaceProvider::Query() {
    auto sharedPtr = ptr.lock();

    return dynamic_pointer_cast<T>(ptr);
}

}
