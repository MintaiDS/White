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
    void DisplayPost(int node);
    void DisplayEdge(int edge);
    void DisplayTrain(int train);
    void SetRenderer( White::Engine::Graphics::Renderer* renderer);
    void SetGraph(std::shared_ptr<White::Util::Math::Graph> graph);
    void SetGrid(std::shared_ptr<White::Grid> grid); 

protected:
    unsigned mainMesh; 
    unsigned mainModel;
    White::Engine::Graphics::Renderer* renderer;
    std::shared_ptr<White::Util::Math::Graph> graph;
    std::shared_ptr<White::Grid> grid;
    std::vector<White::Cell> cells;
    std::vector<int> shuffledIndices;
};
