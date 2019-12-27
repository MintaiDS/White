#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "ModelFormat.h"

#include <memory>

namespace White {
namespace Engine {
namespace Graphics {

class Model : public Object {
public:
    Model();

    unsigned GetMesh() const;
    Texture& GetTexture();
    ModelFormat GetFormat() const;
    void SetMesh(unsigned mesh);
    void SetTexture(Texture texture);
    void SetFormat(ModelFormat format);

protected:
    unsigned mesh;
    Texture texture;
    ModelFormat format;
};

}
}
}
