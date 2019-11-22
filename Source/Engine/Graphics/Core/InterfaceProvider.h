#pragma once

#include "Object.h"
#include "ObjectManager.h"

#include <memory>

using namespace White::Engine;

namespace White {

class InterfaceProvider {
public:
    InterfaceProvider(unsigned id = 0);

    template<typename T> 
    std::shared_ptr<T> Query();
    template<typename T> 
    std::shared_ptr<T> Query(unsigned id);

protected:
    unsigned id;
};

template<typename T>
std::shared_ptr<T> InterfaceProvider::Query() {
    ObjectManager& om = ObjectManager::GetInstance();
    auto sharedPtr = om.GetObjectById(id).lock();

    return std::dynamic_pointer_cast<T>(sharedPtr);
}

template<typename T>
std::shared_ptr<T> InterfaceProvider::Query(unsigned id) {
    ObjectManager& om = ObjectManager::GetInstance();
    auto sharedPtr = om.GetObjectById(id).lock();

    return std::dynamic_pointer_cast<T>(sharedPtr);
}

}
