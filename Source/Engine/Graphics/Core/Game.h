#pragma once

#include "Renderer.h"

namespace White {
namespace Engine {

class Game {
public:
    Game(Graphics::Renderer& renderer);
    virtual void Play();

protected:
    Graphics::Renderer& renderer;
};

}
}
