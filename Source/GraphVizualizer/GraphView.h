#pragma once

#include "Graph.h"
#include "Vector.h"
#include "Grid.h"
#include "Renderer.h"

class GraphView {
public:
    GraphView();

    void Init();
    void Display();
    void DisplayNode(int node);
    void DisplayEdge(int from, int to);
    void SetRenderer(White::Engine::Graphics::Renderer* renderer);
    void SetGraph(White::Util::Math::Graph* graph);
    void SetGrid(White::Grid* grid); 

protected:
    White::Engine::Graphics::Renderer* renderer;
    White::Util::Math::Graph* graph;
    White::Grid* grid;
    std::vector<White::Cell> cells;
};
