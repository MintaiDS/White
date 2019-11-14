#pragma once

#include "Game.h"
#include "Graph.h"
#include "Vector.h"
#include "GraphView.h"

#include <string>

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Graphics {

class GraphVisualizer : Game {
public:
    GraphVisualizer(Renderer& renderer);

    void LoadGraph(std::string path);

    virtual void Play();

protected:
    std::shared_ptr<Graph> graph;
    std::shared_ptr<Grid> grid;
    GraphView graphView;
};

}
}
}
