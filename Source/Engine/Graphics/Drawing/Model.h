#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "ModelFormat.h"

namespace White {
namespace Engine {
namespace Graphics {

class Model : public Object {
public:
    Model();

    void SetMesh(Mesh<float> mesh);
    void SetTexture(Texture texture);
    void SetFormat(ModelFormat format);
    ModelFormat GetFormat() const;

protected:
    Mesh<float> mesh;
    Texture texture;
    ModelFormat format;
};

}
}
}
