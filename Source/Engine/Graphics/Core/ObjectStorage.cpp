#include "ObjectStorage.h"

namespace White {

std::weak_ptr<Object> ObjectStorage::GetObjectById(unsigned id) {
    return objects[id];
}

void ObjectStorage::AddObject(std::weak_ptr<Object> object) {
    objects.push_back(object.lock());
}

}
