#include "CharacterBlock.h"

#include <memory>

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Graphics {

CharacterBlock::CharacterBlock(const Vector<float>& blockPosition,
                               const Vector<float>& cellSize, std::string str) 
        : str(str)
        , translation(blockPosition)
        , rotation({0.0f, 0.0f, 0.0f})
        , scaling({1.0f, 1.0f, 1.0f}) {
    DigitMeshCreator digitMeshCreator;
    digitMeshCreator.Load();
    grid = std::make_unique<Grid>(
                    Vector<float>{0.0f, 0.0f}, 
                    Vector<int>{static_cast<int>(str.length()), 1}, cellSize);
    for (int i = 0; i < str.length(); i++) {
        Mesh<float> mesh = digitMeshCreator.GetMeshForDigit(str[i] - '0');
        Vector<float> vPos = grid->cells[0][i].vertexPosition;
        vPos = {vPos[0], vPos[1], 0.0f};
        mesh.Translate(vPos); 
        meshes.push_back(mesh);
    }
}

std::vector<Mesh<float>>& CharacterBlock::GetMeshes() {
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i].Translate(translation);
        meshes[i].Rotate(rotation);
        meshes[i].Scale(scaling);
    }

    return meshes;
}

void CharacterBlock::Translate(const Vector<GLfloat>& translation) {
    this->translation += translation;
}

void CharacterBlock::Rotate(const Vector<GLfloat>& rotation) {
    this->rotation += rotation;
}

void CharacterBlock::Scale(const Vector<GLfloat>& scaling) {
    this->scaling = scaling;
}

}
}
}
