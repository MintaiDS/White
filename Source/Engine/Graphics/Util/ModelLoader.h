#pragma once

#include "TextureLoader.h"
#include "ModelFormat.h"
#include "MeshLoader.h"
#include "Model.h"

namespace White {
namespace Engine {
namespace Graphics {

class ModelLoader {
public:
    ModelLoader(std::wstring dir = L""); 

    Model Import(std::wstring filename);
    void Export(std::wstring filename);
    ModelFormat ImportFormat(std::wstring filename);
    void ImportMesh(std::wstring meshPath = L"");
    void ImportTexture(std::wstring texturePath = L"");
    void SetDirectory(std::wstring dir);
    Model GetModel();

protected:
    Model model;
    std::wstring dir;
    TextureLoader textureLoader;
    MeshLoader meshLoader;
};

}
}
}
