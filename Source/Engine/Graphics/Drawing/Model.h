#pragma once

namespace White {
namespace Engine {
namespace Graphics {

class Model : public Object, public ITransformable {
public:
    Model();

    void SetMesh(unsigned mesh);
    void SetTexture(unsigned texture);

protected:
    unsigned mesh;
    unsigned texture;
};

}
}
}
