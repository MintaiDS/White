#include "DigitMeshCreator.h"

#include <sstream>
#include <fstream>

namespace White {
namespace Engine {
namespace Graphics {

void DigitMeshCreator::Load() {
    MeshLoader meshLoader;
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
