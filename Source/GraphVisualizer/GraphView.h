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
    void DisplayEdge(Edge* edge);
    void DisplayTrain(Train* train);
    void UpdateTrains();
    void SetRenderer( White::Engine::Graphics::Renderer* renderer);
    void SetGraph(std::shared_ptr<White::Util::Math::Graph> graph);
    void SetGrid(std::shared_ptr<White::Grid> grid); 

protected:
    unsigned mainMesh; 
    unsigned mainModel;
    White::Engine::Graphics::Renderer* renderer;
    std::shared_ptr<White::Util::Math::Graph> graph;
    std::shared_ptr<White::Grid> grid;
    std::vector<unsigned> trains;
    std::vector<White::Cell> cells;
    std::vector<int> shuffledIndices;
};
