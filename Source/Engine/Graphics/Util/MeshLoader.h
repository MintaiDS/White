#include "Mesh.h"

#include <string>

namespace White {
namespace Engine {
namespace Graphics {

struct MeshLoader {
    MeshLoader();
    MeshLoader(const Mesh<GLfloat>& mesh);

    Mesh<float> Import(std::wstring path);
    void Export(std::wstring path, const Mesh<float>& mesh);

    Mesh<float> mesh;
};

}
}
}
