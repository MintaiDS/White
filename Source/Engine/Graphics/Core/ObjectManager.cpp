#include "ObjectManager.h"

namespace White {
namespace Engine {

ObjectManager::ObjectManager() {}

ObjectManager::~ObjectManager() {}

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

}
}
