#pragma once

#include "ObjectStorage.h"

#include <memory>

namespace White {

class ObjectManager {
public:
    static ObjectManager& GetInstance();
    std::weak_ptr<Object> GetObjectById(unsigned id);
    void AddObject(std::weak_ptr<Object> object);
    template<typename T, typename... Args> 
    std::shared_ptr<T> Create(Args&&... args);

protected:
    ObjectManager();
    ~ObjectManager();
    ObjectManager(const ObjectManager& other) = delete;
    ObjectManager& operator=(const ObjectManager& other) = delete;

    ObjectStorage storage;
};

}
