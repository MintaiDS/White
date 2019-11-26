#include "Model.h"

namespace White {
namespace Engine {
namespace Graphics {

Model::Model() : mesh(mesh), texture(texture) {}

void Model::SetMesh(unsigned mesh) {
    this->mesh = mesh;
}

void Model::SetTexture(unsigned texture) {
    this->texture = texture;
}

}
}
}
