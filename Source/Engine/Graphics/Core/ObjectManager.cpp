#include "ObjectManager.h"

namespace White {

ObjectManager& ObjectManager::GetInstance() {
    static ObjectManager objectManager;

    return objectManager;
}

std::weak_ptr<Object> ObjectManager::GetObjectById(unsigned id) {
    return storage.GetObjectById(id);
}

void ObjectManager::AddObject(std::weak_ptr<Object> object) {
    storage.AddObject(object);
}

template<typename T, typename... Args>
std::shared_ptr<T> ObjectManager::Create(Args&&...) {
    std::shared_ptr<T> sharedPtr = std::make_shared<T>(Args);
    AddObject(sharedPtr);

    return sharedPtr;
}

}
