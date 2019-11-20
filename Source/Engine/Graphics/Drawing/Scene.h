#pragma once

#include <vector>

namespace White {
namespace Engine {

class Scene {
public:
    Scene();

    void AddObject(unsigned object);

protected: 
    std::vector<unsigned> objects;
};

}
}
