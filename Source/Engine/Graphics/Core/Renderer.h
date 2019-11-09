#pragma once

#include "Object.h"

#include <vector>

namespace White {
namespace Engine {
namespace Graphics {

class Renderer {
public:
    void Render();

private:
    std::vector<Object> objects;
};

}
}
}
