#pragma once

#include "Game.h"
#include "Graph.h"
#include "Vector.h"

#include <string>

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Graphics {

class GraphVizualizer : Game {
public:
    GraphVizualizer(Renderer& renderer);

    void LoadGraph(std::string path);
    void DisplayNode(Vector<GLfloat>& pos);

    virtual void Play();

protected:
    Graph* graph;
    std::vector<Vector<GLfloat>> grid;
};

}
}
}
