#include "ModelFormat.h"

#include <algorithm>

namespace White {
namespace Engine {
namespace Graphics {

ModelFormat::ModelFormat() 
        : numAttributes(0)
        , numShaders(0)
        , isIndexed(false)
        , isTextured(false) {}

bool ModelFormat::operator==(const ModelFormat& other) const {
    bool ret = numAttributes == other.numAttributes;
    ret &= numShaders == other.numShaders;
    ret &= isIndexed == other.isIndexed;
    ret &= isTextured == other.isTextured;
    for (auto i = 0; i < std::min(numAttributes, other.numAttributes); i++) {
        ret &= numComponents[i] == other.numComponents[i];
    }
    for (auto i = 0; i < std::min(numShaders, other.numShaders); i++) {
        ret &= shaders[i] == other.shaders[i];
    }

    return ret;
}

}
}
}
