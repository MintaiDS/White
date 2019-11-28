#include "Model.h"

namespace White {
namespace Engine {
namespace Graphics {

Model::Model() {}

unsigned Model::GetMesh() const {
    return mesh;
}

Texture& Model::GetTexture() {
    return texture;
}

ModelFormat Model::GetFormat() const {
    return format;
}

void Model::SetMesh(unsigned mesh) {
    this->mesh = mesh;
}

void Model::SetTexture(Texture texture) {
    this->texture = texture;
}

void Model::SetFormat(ModelFormat format) {
    this->format = format;
}

}
}
}
