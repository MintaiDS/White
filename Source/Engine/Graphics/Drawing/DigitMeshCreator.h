#pragma once

#include "MeshLoader.h"

namespace White {
namespace Engine {
namespace Graphics {

class DigitMeshCreator {
public:
    void Load();
    Mesh<float> GetMeshForDigit(int digit);

protected:
    std::vector<Mesh<float>> digitMeshes;
};

}
}
}
