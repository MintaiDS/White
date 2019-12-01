#include "DigitMeshCreator.h"
#include "ModelFormat.h"

#include <sstream>
#include <fstream>

namespace White {
namespace Engine {
namespace Graphics {

void DigitMeshCreator::Load() {
    MeshLoader meshLoader;
    ModelFormat format;
    format.numAttributes = 2;
    format.numShaders = 2;
    format.isTextured = false;
    format.isIndexed = true;
    format.numComponents.push_back(4);
    format.numComponents.push_back(4);
    format.shaders.push_back(L"Engine/Shaders/default.vsh");
    format.shaders.push_back(L"Engine/Shaders/default.fsh");
    meshLoader.format = format;
    for (int i = 0; i < 10; i++) {
        std::wstringstream stream;
        stream << L"Engine/Models/Digits/" << L"digit_" << i << L".polygon";
        std::wstring path(stream.str());
        meshLoader.Import(path);
        digitMeshes.push_back(meshLoader.mesh);
    }
}

Mesh<float> DigitMeshCreator::GetMeshForDigit(int digit) {
    return digitMeshes[digit];
}

}
}
}
