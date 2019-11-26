#include "Model.h"

namespace White {
namespace Engine {
namespace Graphics {

Model::Model() {}

void Model::SetMesh(Mesh<float> mesh) {
    this->mesh = mesh;
}

void Model::SetTexture(Texture texture) {
    this->texture = texture;
}

void Model::SetFormat(ModelFormat format) {
    this->format = format;
}

ModelFormat Model::GetFormat() const {
    return format;
}

}
}
}
