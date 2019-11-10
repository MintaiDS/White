#pragma once

#include "MeshManager.h"
#include "Renderer.h"

namespace White {
namespace Engine {
namespace Graphics {

struct Core {
    Core();

    MeshManager meshManager;
    Renderer renderer;
};

}
}
}
