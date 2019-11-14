#pragma once

namespace White {
namespace Engine {
namespace Graphics {

class DigitMeshCreator {
public:
    void Load();

protected:
    std::vector<Mesh<float>> digitMeshes;
};

}
}
}
