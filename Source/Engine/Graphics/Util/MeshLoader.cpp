#include "MeshLoader.h"

namespace White {
namespace Engine {
namespace Graphics {

class MeshLoader {
    void Import(std::wstring path);
    void Export(std::wstring path);

    Mesh<GLfloat> mesh;
};

}
}
}
