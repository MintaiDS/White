#pragma once

#include "Game.h"

namespace White {
namespace Engine {
namespace Graphics {

class GraphVizualizer : Game {
public:
    GraphVizualizer(Renderer& renderer);

    virtual void Play();
};

}
}
}
