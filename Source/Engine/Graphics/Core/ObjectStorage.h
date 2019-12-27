#pragma once

#include "Object.h"

#include <vector>
#include <memory>

namespace White {

class ObjectStorage {
public:
    std::weak_ptr<Object> GetObjectById(unsigned id); 
    void AddObject(std::weak_ptr<Object> object);

protected:
    std::vector<std::shared_ptr<Object>> objects;
};

}
