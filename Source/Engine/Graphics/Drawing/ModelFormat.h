#pragma once

#include <string>
#include <vector>

namespace White {
namespace Engine {
namespace Graphics {

struct ModelFormat {
    ModelFormat();

    int numAttributes;
    int numShaders;
    bool isIndexed;
    bool isTextured;
    std::vector<unsigned> numComponents;
    std::vector<std::wstring> shaders;
};

}
}
}
