#pragma once

#include "Renderer.h"

namespace White {
namespace Engine {
namespace Graphics {

class Renderer;

class Game {
public:
    Game(Renderer& renderer);
    virtual void Play();

protected:
    Renderer& renderer;
};

}
}
}
