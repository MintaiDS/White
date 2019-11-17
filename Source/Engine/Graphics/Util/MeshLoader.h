#include "Mesh.h"

namespace White {
namespace Engine {
namespace Graphics {

struct MeshLoader {
    MeshLoader();
    MeshLoader(const Mesh<GLfloat>& mesh);

    void Import(std::wstring path);
    void Export(std::wstring path, const Mesh<GLfloat>& mesh);

    Mesh<GLfloat> mesh;
};

}
}
}
