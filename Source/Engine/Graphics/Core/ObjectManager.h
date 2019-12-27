#pragma once

#include "ObjectStorage.h"
#include "ObjectIDProvider.h"

#include <memory>

namespace White {
namespace Engine {

class ObjectManager {
public:
    static ObjectManager& GetInstance();
    std::weak_ptr<Object> GetObjectById(unsigned id);
    void AddObject(std::weak_ptr<Object> object);
    template<class T, class... Args> 
    unsigned Create(Args&&... args);

protected:
    ObjectManager();
    ~ObjectManager();
    ObjectManager(const ObjectManager& other) = delete;
    ObjectManager& operator=(const ObjectManager& other) = delete;

    ObjectStorage storage;
    ObjectIDProvider<unsigned> idProvider;
};

template<typename T, typename... Args>
unsigned ObjectManager::Create(Args&&... args) {
    unsigned ret = idProvider.GetNextID();
    std::shared_ptr<T> sharedPtr = std::make_shared<T>(args...);
    AddObject(sharedPtr);

    return ret;
}

}
}
