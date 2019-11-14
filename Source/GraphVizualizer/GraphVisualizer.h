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
    void DisplayNode(Vector<GLfloat>& pos);
    void DisplayEdge(Vector<GLfloat>& begin, Vector<GLfloat>& end);

    virtual void Play();

protected:
    Graph* graph;
    //std::vector<Vector<GLfloat>> grid;
    Grid* grid;
    GraphView graphView;
};

}
}
}
