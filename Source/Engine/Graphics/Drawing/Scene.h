#pragma once

#include "IndexProvider.h"

#include <vector>

namespace White {
namespace Engine {

class Scene {
public:
    Scene();

    void AddObject(unsigned object);
    void RemoveObject(unsigned object);

protected: 
    std::vector<unsigned> objects;
    IndexProvider<> ip;
};

}
}
