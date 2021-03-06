#include "ModelLoader.h"
#include "ObjectManager.h" 
#include "Logger.h"
#include "InterfaceProvider.h"

#include <fstream>

using namespace White::Util;

namespace White {
namespace Engine {
namespace Graphics {

ModelLoader::ModelLoader(std::wstring dir) : dir(dir) {}

Model ModelLoader::Import(std::wstring filename) {
    std::wstring formatPath;
    std::wstring meshPath;
    std::wstring texturePath;
    std::wstring transformPath;
    std::wifstream modelFile(filename);
    modelFile >> formatPath;
    ModelFormat format = ImportFormat(formatPath); 
    model.SetFormat(format);
    modelFile >> meshPath;
    modelFile >> texturePath;
    modelFile >> transformPath;
    ImportMesh(meshPath);
    ImportTexture(texturePath);
    ImportTransform(transformPath);

    return model;
}

ModelFormat ModelLoader::ImportFormat(std::wstring filename) {
    ModelFormat format;
    int boolValue;
    std::wifstream in(filename);
    in >> format.numAttributes;
    in >> format.numShaders;
    in >> boolValue;
    format.isIndexed = boolValue != 0;
    in >> boolValue;
    format.isTextured = boolValue != 0; 
    for (auto i = 0; i < format.numAttributes; i++) {
        unsigned numComponents;
        in >> numComponents;
        format.numComponents.push_back(numComponents);
    }
    for (auto i = 0; i < format.numShaders; i++) {
        std::wstring shaderSourcePath;
        in >> shaderSourcePath;
        format.shaders.push_back(shaderSourcePath);
    }
    in.close();

    return format;
}

void ModelLoader::ImportMesh(std::wstring meshPath) {
    ObjectManager& om = ObjectManager::GetInstance();
    meshLoader.format = model.GetFormat();
    unsigned mesh = om.Create<Mesh<float>>(meshLoader.Import(meshPath));
    model.SetMesh(mesh);
}

void ModelLoader::ImportTexture(std::wstring texturePath) {
    if (model.GetFormat().isTextured) {
        model.SetTexture(textureLoader.Import(texturePath));
    }
}

void ModelLoader::ImportTransform(std::wstring transformPath) {
    std::wifstream in(transformPath);
    Vector<float> scaling(3);
    Vector<float> rotation(3);
    Vector<float> translation(3);
    in >> scaling[0] >> scaling[1] >> scaling[2];
    in >> rotation[0] >> rotation[1] >> rotation[2];
    in >> translation[0] >> translation[1] >> translation[2];
    in.close();
    InterfaceProvider ip;
    ip.Query<ITransformable>(model.GetMesh())->Transform<float>(scaling, 
                                                                rotation, 
                                                                translation);
}

void ModelLoader::Export(std::wstring filename) {}

void ModelLoader::SetDirectory(std::wstring dir) {
    this->dir = dir;
}

Model ModelLoader::GetModel() {
    return model;
}

}
}
}
